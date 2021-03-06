/*
 * $Id: chess.cpp 1336 2014-12-08 09:29:59Z justin $
 * Copyright (C) 2009 Lucid Fusion Labs

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "core/app/app.h"
#include "core/app/shell.h"
#include "core/app/gl/view.h"
#include "core/app/gl/terminal.h"
#include "core/app/ipc.h"
#include "core/app/net/resolver.h"

namespace LFL {
DEFINE_string(connect, "freechess.org:5000", "Connect to server");
DEFINE_bool(click_or_drag_pieces, MOBILE, "Move by clicking or dragging");
DEFINE_bool(auto_close_old_games, true, "Close old games whenever new game starts");
DEFINE_string(seek_command, "5 0", "Seek command");
DEFINE_string(engine, "", "Chess engine");

struct MyApp : public Application {
  using Application::Application;
  point initial_board_dim = point(630, 630);
  point initial_term_dim = point(initial_board_dim.x / Fonts::InitFontWidth(), 10);
  unique_ptr<AlertViewInterface> askseek, askresign;
  unique_ptr<MenuViewInterface> editmenu, viewmenu, gamemenu;
  unique_ptr<ToolbarViewInterface> maintoolbar;
  void OnWindowInit(Window *W);
  void OnWindowStart(Window *W);
} *app;

inline string   LS  (const char *n) { return app->GetLocalizedString(n); }
inline String16 LS16(const char *n) { return app->GetLocalizedString16(n); }
};

#ifdef LFL_FLATBUFFERS
#include "LTerminal/term_generated.h"
#endif
#include "LTerminal/term.h"
#include "chess.h"
#include "fics.h"

namespace LFL {
struct ChessTerminalTab : public TerminalTabT<ChessTerminal> {
  using TerminalTabT::TerminalTabT;
  virtual bool GetFocused() const { return true; }
  virtual bool Animating() const { return false; }
  virtual void UpdateTargetFPS() {}
  virtual void SetFontSize(int) {}
  virtual void DrawBox(GraphicsDevice*, Box draw_box, bool check_resized) {}
};

struct ChessView : public View {
  unique_ptr<UniversalChessInterfaceEngine> chess_engine;
  unique_ptr<ChessTerminalTab> chess_terminal;
  point term_dim=app->initial_term_dim;
  v2 square_dim;
  Box win, board, term;
  Widget::Divider divider;
  Chess::Game *top_game=0;
  unordered_map<int, Chess::Game> game_map;
  bool title_changed = 0, console_animating = 0;
  Time move_animation_time = Time(200);
  DragTracker drag_tracker;
  ChessView(Window *W) : View(W, "ChessView"), divider(this, true, W->gl_w) { (top_game = &game_map[0])->active = 0; }

  /**/  Chess::Game *Top()       { return top_game; }
  const Chess::Game *Top() const { return top_game; }

  Box SquareCoords(int p, bool flip_board) const {
    int sx = Chess::SquareX(p), sy = Chess::SquareY(p);
    return Box(board.x + (flip_board ? (7-sx) : sx) * square_dim.x,
               board.y + (flip_board ? (7-sy) : sy) * square_dim.y, square_dim.x, square_dim.y, true);
  }

  int SquareFromCoords(const point &p, bool flip_board) const {
    int sx = p.x / square_dim.x, sy = p.y / square_dim.y;
    return Chess::SquareFromXY(flip_board ? (7-sx) : sx, flip_board ? (7-sy) : sy);
  }

  void UseChessTerminalController(string hostport) {
    INFO("connecting to ", hostport);
    auto c = make_unique<NetworkTerminalController>
      (chess_terminal.get(), hostport, bind(&ChessView::UseReconnectTerminalController, this, hostport));
    c->frame_on_keyboard_input = true;
    chess_terminal->ChangeController(move(c));
    chess_terminal->terminal->controller = chess_terminal->controller.get();
  }

  void UseReconnectTerminalController(string hostport) {
    auto c = make_unique<BufferedShellTerminalController>
      (chess_terminal.get(), &root->parent->localfs, "\r\nConnection closed.\r\n", StringCB(), StringVecCB(),
       bind(&ChessView::UseChessTerminalController, this, hostport), false);
    c->enter_char = '\n';
    chess_terminal->ChangeController(move(c));
    chess_terminal->terminal->controller = chess_terminal->controller.get();
  }

  void Open(const string &hostport) {
    Activate();
    if (hostport.size()) UseChessTerminalController(hostport);

    auto t = chess_terminal->terminal;
#ifdef LFL_MOBILE
    t->drag_cb         = [=](int, point, point, int d){ if (d) app->ToggleTouchKeyboard(); return true; };
#endif
    t->get_game_cb     = [=](int game_no){ return &game_map[game_no]; };
    t->illegal_move_cb = bind(&ChessView::IllegalMoveCB, this);
    t->login_cb        = bind(&ChessView::LoginCB,       this);
    t->game_start_cb   = bind(&ChessView::GameStartCB,   this, _1);
    t->game_over_cb    = bind(&ChessView::GameOverCB,    this, _1, _2, _3, _4);
    t->game_update_cb  = bind(&ChessView::GameUpdateCB,  this, _1, _2, _3, _4);
    t->SetColors(Singleton<Terminal::StandardVGAColors>::Set());
  }

  void Send(const string &b) {
    ChessTerminal *t;
    if ((t = chess_terminal->terminal) && t->controller) t->Send(b);
    else if (auto e = chess_engine.get()) {}
  }

  void LoadPosition(const vector<string> &arg) {
    auto g = Top();
    if (!g || !arg.size()) return;
    if (arg.size() == 1) {
      if      (arg[0] == "kiwipete")  g->last_position.LoadByteBoard(Chess::kiwipete_byte_board);
      else if (arg[0] == "perftpos3") g->last_position.LoadFEN      (Chess::perft_pos3_fen);
    } else if (g->last_position.LoadFEN(Join(arg, " "))) {}
    g->position = g->last_position;
  }

  void ListGames() { for (auto &i : game_map) INFO(i.first, " ", i.second.p1_name, " vs ", i.second.p2_name); }
  void FlipBoard(Window *w) { if (auto g = Top()) g->flip_board = !g->flip_board; w->Wakeup(); }
  void UpdateAnimating(Window *w) { app->scheduler.SetAnimating(w, (Top() && Top()->move_animate_from != -1) | console_animating); }

  void ConsoleAnimatingCB() {
    console_animating = root ? root->console->animating : 0;
    UpdateAnimating(root);
  }

  void LoginCB() { root->SetCaption(StrCat(chess_terminal->terminal->my_name, " @ ", FLAGS_connect)); }

  void GameStartCB(int game_no) {
    Chess::Game *game = top_game = &game_map[game_no];
    game->Reset();
    game->game_number = game_no;
    if (FLAGS_enable_audio) {
      static SoundAsset *start_sound = app->soundasset("start");
      root->parent->audio->PlaySoundEffect(start_sound);
    }
    if (FLAGS_auto_close_old_games) FilterValues<unordered_map<int, Chess::Game>>
      (&game_map, [](const pair<const int, Chess::Game> &x){ return !x.second.active; });
  }

  void GameOverCB(int game_no, const string &p1, const string &p2, const string &result) {
    const string &my_name = chess_terminal->terminal->my_name;
    bool lose = (my_name == p1 && result == "0-1") || (my_name == p2 && result == "1-0");
    if (FLAGS_enable_audio) {
      static SoundAsset *win_sound = app->soundasset("win"), *lose_sound = app->soundasset("lose");
      root->parent->audio->PlaySoundEffect(lose ? lose_sound : win_sound);
    }
    game_map[game_no].active = false;
  }

  void GameUpdateCB(Chess::Game *game, bool reapply_premove = false, int animate_from = -1, int animate_to = -1) {
    top_game = game;
    title_changed = true;
    if (FLAGS_enable_audio) {
      static SoundAsset *move_sound = app->soundasset("move"), *capture_sound = app->soundasset("capture");
      root->parent->audio->PlaySoundEffect(Chess::GetMoveCapture(game->position.move) ? capture_sound : move_sound);
    }
    if (game->new_game && !(game->new_game = 0)) {
      game->flip_board = game->my_color == Chess::BLACK;
    }
    if (reapply_premove) ReapplyPremoves(game);
    if (animate_from != -1 && animate_to != -1 &&
        Chess::GetPieceType((game->animating_piece = game->position.ClearSquare(animate_to)))) {
      game->move_animate_from = animate_from;
      game->move_animate_to = animate_to;
      game->move_animation_start = Now();
    } else game->move_animate_from = -1;
    UpdateAnimating(root);
  }

  void IllegalMoveCB() {
    if (FLAGS_enable_audio) {
      static SoundAsset *illegal_sound = app->soundasset("illegal");
      root->parent->audio->PlaySoundEffect(illegal_sound);
    }
  }

  void ClickCB(int button, point p, point d, int down) {
    Chess::Game *game = Top();
    if (!game || !down) return;
    p -= board.Position();
    int square, start_square;
    Chess::Piece moved_piece=0;
    if ((square = SquareFromCoords(p, game->flip_board)) < 0) return;
    if (!Chess::GetPieceType(game->moving_piece)) {
      drag_tracker.beg_click = drag_tracker.end_click = p;
      game->moving_piece = game->position.ClearSquare(square);
      return;
    }
    swap(game->moving_piece, moved_piece);
    game->position.SetSquare(square, moved_piece);
    if ((start_square = SquareFromCoords(drag_tracker.beg_click, game->flip_board)) == square) return;
    MakeMove(game, Chess::GetPieceType(moved_piece), start_square, square);
  }

  void DragCB(int button, point p, point d, int down) {
    Chess::Game *game = Top();
    if (!game) return;
    p -= board.Position();
    int square, start_square;
    Chess::Piece moved_piece=0;
    if ((square = SquareFromCoords(p, game->flip_board)) < 0) return;
    if (drag_tracker.Update(p, down)) game->moving_piece = game->position.ClearSquare(square);
    if (!Chess::GetPieceType(game->moving_piece) || down) return;
    swap(game->moving_piece, moved_piece);
    game->position.SetSquare(square, moved_piece);
    if ((start_square = SquareFromCoords(drag_tracker.beg_click, game->flip_board)) == square) return;
    MakeMove(game, Chess::GetPieceType(moved_piece), start_square, square);
  }

  void MakeMove(Chess::Game *game, int piece, int start_square, int end_square, bool animate=false) {
    ChessTerminal *t=0;
    if ((t = chess_terminal->terminal) && t->controller) {
      string move = StrCat(Chess::PieceChar(piece), Chess::SquareName(start_square), Chess::SquareName(end_square));
      if (game->position.flags.to_move_color == game->my_color) t->MakeMove(move);
      else {
        auto &pm = PushBack(game->premove, game->position);
        pm.name = move;
        pm.move = Chess::GetMove(piece, start_square, end_square, 0, 0, 0);
      }
    } else if (auto e = chess_engine.get()) {
      if (game->position.PlayerIllegalMove(piece, start_square, end_square, game->last_position)) {
        IllegalMoveCB();
        game->position = game->last_position;
      } else {
        if (game->history.empty()) {
          chess_terminal->terminal->my_name = game->p1_name = "Player1";
          game->p2_name = "Player2"; 
        }
        game->active = true;
        game->update_time = Now();
        game->position.name = StrCat(Chess::PieceChar(piece), Chess::SquareName(start_square), Chess::SquareName(end_square));
        game->position.PlayerMakeMove(piece, start_square, end_square, game->last_position);
        game->AddNewMove();
        GameUpdateCB(game, animate, animate ? start_square : -1, animate ? end_square : -1);
      }
    }
  }

  void Reshaped() { divider.size = root->gl_w; }
  View *Layout(Flow *flow_in=nullptr) override {
    Font *font = chess_terminal->terminal->style.font;
    ResetView();
    win = root->ViewBox();
    term.w = win.w;
    term_dim.x = win.w / font->FixedWidth();
    int min_term_h = font->Height() * 3;
    divider.max_size = min(win.w, win.h - min_term_h);
    divider.LayoutDivideTop(win, &win, &term);
    CHECK_LE(win.h, win.w);
    if (int d = win.w - win.h) { win.w -= d; win.x += d/2; }
    CHECK_EQ(win.w, win.h);
    board = Box::DelBorder(win, Border(5,5,5,5));
    square_dim = v2(board.w/8.0, board.h/8.0);
    if (FLAGS_click_or_drag_pieces) mouse.AddClickBox(board, MouseController::CoordCB(bind(&ChessView::ClickCB, this, _1, _2, _3, _4)));
    else                            mouse.AddDragBox (board, MouseController::CoordCB(bind(&ChessView::DragCB,  this, _1, _2, _3, _4)));

    Texture *board_tex = &app->asset("board1")->tex;
    child_box.PushBack(win, Drawable::Attr(board_tex), board_tex);
    return this;
  }

  int Frame(LFL::Window *W, unsigned clicks, int flag) {
    Chess::Game *game = Top();
    GraphicsContext gc(W->gd);
    point p = W->Box().TopLeft();
    Time now = Now();

    if (game && (title_changed || game->active)) {
      if (game && game->active) {
        int secs = game->position.move_number ? ToSeconds(now - game->update_time).count() : 0;
        game->last_p1_secs = game->p1_secs - (game->position.flags.to_move_color ? 0 : secs);
        game->last_p2_secs = game->p2_secs - (game->position.flags.to_move_color ? secs : 0);
      }
      string title = StringPrintf("%s %d:%02d vs %s %d:%02d",
                                 game->p1_name.c_str(), game->last_p1_secs/60, game->last_p1_secs%60,
                                 game->p2_name.c_str(), game->last_p2_secs/60, game->last_p2_secs%60);
      if (game->position.move_number)
        StringAppendf(&title, ": %d%s: %s", 
                      game->position.StandardMoveNumber(),
                      game->position.StandardMoveSuffix(), game->position.name.c_str());

      W->SetCaption(title);
      title_changed = false;
    }

    if (divider.changed) Layout();
    if (win.w != W->gl_w) { ScopedFillColor sfc(W->gd, Color::grey70); gc.DrawBox1(Box(W->gl_x, win.y, W->gl_w, win.h)+p); }
    Draw(p);
    DrawGame(W, p, game ? game : Singleton<Chess::Game>::Set(), now);

    W->gd->DisableBlend();
    {
      Scissor s(W->gd, term+p);
      chess_terminal->terminal->Draw(term+p);
      if (chess_terminal->terminal->scrolled_lines) chess_terminal->DrawScrollBar(term+p);
    }
    if (divider.changing) BoxOutline().Draw(&gc, Box::DelBorder(term, Border(1,1,1,1))+p);

    W->DrawDialogs();
    return 0;
  }

  void DrawGame(Window *W, const point &p, Chess::Game *game, Time now) {
    int black_font_index[7] = { 0, 3, 2, 0, 5, 4, 1 }, bits[65];
    static Font *pieces = app->fonts->Get(W->gl_h, "ChessPieces1");
    Drawable::Attr draw_attr(pieces);
    GraphicsContext gc(W->gd);
    for (int i=Chess::PAWN; i <= Chess::KING; i++) {
      Bit::Indices(game->position.white[i], bits); for (int *b = bits; *b != -1; b++) pieces->DrawGlyph(W->gd, black_font_index[i]+6, SquareCoords(*b, game->flip_board)+p);
      Bit::Indices(game->position.black[i], bits); for (int *b = bits; *b != -1; b++) pieces->DrawGlyph(W->gd, black_font_index[i],   SquareCoords(*b, game->flip_board)+p);
    }

    if (game->position.move_number) {
      W->gd->SetColor(Color(85, 85,  255)); BoxOutline().Draw(&gc, SquareCoords(Chess::GetMoveFromSquare(game->position.move), game->flip_board)+p);
      W->gd->SetColor(Color(85, 255, 255)); BoxOutline().Draw(&gc, SquareCoords(Chess::GetMoveToSquare  (game->position.move), game->flip_board)+p);
    }

    for (auto &pm : game->premove) {
      W->gd->SetColor(Color(255, 85,  85)); BoxOutline().Draw(&gc, SquareCoords(Chess::GetMoveFromSquare(pm.move), game->flip_board)+p);
      W->gd->SetColor(Color(255, 255, 85)); BoxOutline().Draw(&gc, SquareCoords(Chess::GetMoveToSquare  (pm.move), game->flip_board)+p);
    }

    if (auto piece_type = Chess::GetPieceType(game->moving_piece)) {
      bool piece_color = Chess::GetPieceColor(game->moving_piece);
      int start_square = SquareFromCoords(drag_tracker.beg_click, game->flip_board);
      Chess::BitBoard moves = game->position.PieceMoves(piece_type, start_square, piece_color, game->position.AllAttacks(!piece_color));
      Bit::Indices(moves, bits);
      W->gd->SetColor(Color(255, 85, 255));
      for (int *b = bits; *b != -1; b++)   BoxOutline().Draw(&gc, SquareCoords(*b, game->flip_board)+p);
      W->gd->SetColor(Color(170, 0, 170)); BoxOutline().Draw(&gc, SquareCoords(start_square, game->flip_board)+p);
      W->gd->SetColor(Color::white);

      int glyph_index = black_font_index[piece_type] + 6*(!piece_color);
      pieces->DrawGlyph(W->gd, glyph_index, SquareCoords(start_square, game->flip_board) + (drag_tracker.end_click - drag_tracker.beg_click) + p);
    } else W->gd->SetColor(Color::white);

    if (game->move_animate_from != -1) {
      if (game->move_animation_start + move_animation_time < now) {
        game->position.SetSquare(game->move_animate_to, game->animating_piece);
        game->move_animate_from = -1;
        UpdateAnimating(W);
      } 
      int glyph_index = black_font_index[Chess::GetPieceType(game->animating_piece)] +
        6*(!Chess::GetPieceColor(game->animating_piece));
      Box start_square = SquareCoords(game->move_animate_from, game->flip_board) + p;
      Box end_square   = SquareCoords(game->move_animate_to,   game->flip_board) + p;
      float percent = min(1.0f, float((now - game->move_animation_start).count()) / move_animation_time.count());
      point slope(end_square.centerX() - start_square.centerX(), end_square.centerY() - start_square.centerY());
      point pos = start_square.center() + slope * percent;
      pieces->DrawGlyph(W->gd, glyph_index, Box(pos.x - start_square.w/2, pos.y - start_square.h/2, start_square.w, start_square.h));
    }
  }

  void WalkHistory(bool backwards) {
    Chess::Game *game = Top();
    if (!game || !game->history.size()) return;
    int last_history_ind = game->history_ind, ind;
    if (backwards) game->history_ind = min<int>(game->history.size() - 1, game->history_ind + 1);
    else           game->history_ind = max<int>(0,                        game->history_ind - 1);
    if (game->history_ind == last_history_ind) return;
    game->position = game->history[game->history.size()-1-game->history_ind];

    if (!backwards) GameUpdateCB(game, !game->history_ind, Chess::GetMoveFromSquare(game->position.move), Chess::GetMoveToSquare(game->position.move));
    else if (Clamp<int>(last_history_ind, 0, game->history.size()-1) == last_history_ind) {
      auto &last_position = game->history[game->history.size()-1-last_history_ind];
      GameUpdateCB(game, !game->history_ind, Chess::GetMoveToSquare(last_position.move), Chess::GetMoveFromSquare(last_position.move));
    }
  }

  void ReapplyPremoves(Chess::Game *game) {
    auto b = game->premove.begin(), e = game->premove.end(), i = b;
    for (/**/; i != e; ++i) {
      auto pm_piece = game->position.ClearSquare(Chess::GetMoveFromSquare(i->move));
      if (!Chess::GetPieceType(pm_piece)) break;
      game->position.SetSquare(Chess::GetMoveToSquare(i->move), pm_piece);
      i->Assign(game->position);
    }
    if (i != e) game->premove.erase(i, e);
  }

  void UndoPremove(Window *W) {
    Chess::Game *game = Top();
    if (!game) return;
    game->history_ind = 0;
    if (game->premove.size()) game->premove.pop_back();
    if (!game->premove.size() && !game->history.size()) return;
    game->position = game->premove.size() ? game->premove.back() : game->history.back();
    GameUpdateCB(game);
    W->Wakeup();
  }

  void CopyPGNToClipboard() {
    Chess::Game *game = Top();
    if (!game) return;
    string text = StrCat("[Site ", FLAGS_connect.substr(0, FLAGS_connect.find(":")), "]\n[Date: ",
                         logfileday(Now()), "\n[White \"", game->p1_name, "\"]\n[Black \"", game->p2_name, "\"]\n\n");
    if (game->history.size())
      for (auto b = game->history.begin()+1, e = game->history.end(), i = b; i != e; ++i)
        StrAppend(&text, ((i - b) % 2) ? "" : StrCat((i-b)/2+1, ". "), i->name, " ");
    StrAppend(&text, "\n");
    app->SetClipboardText(text);
  }

  void StartEngine(bool black_or_white) {
    bool started = false;
    Chess::Game *game = Top();
    if (!chess_engine || game->position.flags.to_move_color != black_or_white) return;
    if (black_or_white) started = Changed(&game->engine_playing_black, true);
    else                started = Changed(&game->engine_playing_white, true);
    // if (!started) return;
    chess_engine->Analyze(game, [=](int square_from, int square_to){
      game->position = game->last_position;                    
      auto piece = game->position.ClearSquare(square_from);
      if (auto piece_type = Chess::GetPieceType(piece)) {
        game->position.SetSquare(square_to, piece);
        MakeMove(game, piece_type, square_from, square_to, true);
      }
      // INFO("bestmove ", Chess::SquareName(square_from), " ", Chess::SquareName(square_to));
    });
  }
};

void MyApp::OnWindowInit(Window *W) {
  W->caption = "Chess";
  W->gl_w = app->initial_board_dim.x;
  W->gl_h = app->initial_board_dim.y + app->initial_term_dim.y * Fonts::InitFontHeight();
}

void MyApp::OnWindowStart(Window *W) {
  if (FLAGS_console) W->InitConsole(Callback());
  ChessView *chess_view = W->AddView(make_unique<ChessView>(W));
  chess_view->chess_terminal = make_unique<ChessTerminalTab>
    (W, W->AddView(make_unique<FICSTerminal>(nullptr, W, W->default_font, chess_view->term_dim)), 0, 0);

  W->reshaped_cb = bind(&ChessView::Reshaped, chess_view);
  W->frame_cb = bind(&ChessView::Frame, chess_view, _1, _2, _3);
  W->default_textbox = [=]{ return app->run ? chess_view->chess_terminal->terminal : nullptr; };
  if (FLAGS_console) W->console->animating_cb = bind(&ChessView::ConsoleAnimatingCB, chess_view);

  W->shell = make_unique<Shell>(W);
  W->shell->Add("games", bind(&ChessView::ListGames,    chess_view));
  W->shell->Add("load",  bind(&ChessView::LoadPosition, chess_view, _1));

  BindMap *binds = W->AddInputController(make_unique<BindMap>());
  binds->Add(Key::Escape,                    Bind::CB(bind(&Shell::quit,    W->shell.get(), vector<string>())));
  binds->Add('6',        Key::Modifier::Cmd, Bind::CB(bind(&Shell::console, W->shell.get(), vector<string>())));
  binds->Add(Key::Up,    Key::Modifier::Cmd, Bind::CB(bind([=](){ chess_view->chess_terminal->terminal->ScrollUp();   W->Wakeup(); })));
  binds->Add(Key::Down,  Key::Modifier::Cmd, Bind::CB(bind([=](){ chess_view->chess_terminal->terminal->ScrollDown(); W->Wakeup(); })));
  binds->Add(Key::Left,  Key::Modifier::Cmd, Bind::CB(bind([=](){ chess_view->WalkHistory(1); W->Wakeup(); })));
  binds->Add(Key::Right, Key::Modifier::Cmd, Bind::CB(bind([=](){ chess_view->WalkHistory(0); W->Wakeup(); })));

  if (FLAGS_engine.size()) {
    chess_view->chess_engine = make_unique<UniversalChessInterfaceEngine>();
    CHECK(chess_view->chess_engine->Start(W->parent->FileName(FLAGS_engine), W));
  }
}

}; // namespace LFL
using namespace LFL;

extern "C" LFApp *MyAppCreate(int argc, const char* const* argv) {
  FLAGS_enable_video = FLAGS_enable_audio = FLAGS_enable_input = FLAGS_enable_network = FLAGS_console = 1;
  FLAGS_console_font = "Nobile.ttf";
  FLAGS_console_font_flag = 0;
  FLAGS_peak_fps = 20;
  FLAGS_target_fps = 0;
  app = make_unique<MyApp>(argc, argv).release();
  app->focused = app->framework->ConstructWindow(app).release();
  app->name = "LChess";
  app->window_start_cb = bind(&MyApp::OnWindowStart, app, _1);
  app->window_init_cb = bind(&MyApp::OnWindowInit, app, _1);
  app->window_init_cb(app->focused);
#ifdef LFL_MOBILE
  app->SetExtraScale(true);
  app->SetTitleBar(true);
  app->SetKeepScreenOn(false);
  app->SetAutoRotateOrientation(true);
  app->CloseTouchKeyboardAfterReturn(false);
#endif
  return app;
}

extern "C" int MyAppMain(LFApp*) {
  if (app->Create(__FILE__)) return -1;
#ifdef WIN32
  app->asset_cache["default.vert"]                                    = app->LoadResource(200);
  app->asset_cache["default.frag"]                                    = app->LoadResource(201);
  app->asset_cache["MenuAtlas,0,255,255,255,0.0000.glyphs.matrix"]    = app->LoadResource(202);
  app->asset_cache["MenuAtlas,0,255,255,255,0.0000.png"]              = app->LoadResource(203);
  app->asset_cache["ChessPieces1,0,255,255,255,0.0000.glyphs.matrix"] = app->LoadResource(206);
  app->asset_cache["ChessPieces1,0,255,255,255,0.0000.png"]           = app->LoadResource(207);
  app->asset_cache["board1.png"]                                      = app->LoadResource(208);
  app->asset_cache["capture.wav"]                                     = app->LoadResource(209);
  app->asset_cache["illegal.wav"]                                     = app->LoadResource(210);
  app->asset_cache["lose.wav"]                                        = app->LoadResource(211);
  app->asset_cache["move.wav"]                                        = app->LoadResource(212);
  app->asset_cache["start.wav"]                                       = app->LoadResource(213);
  app->asset_cache["win.wav"]                                         = app->LoadResource(214);
  if (FLAGS_console) {
    app->asset_cache["Nobile.ttf,32,255,255,255,0.0000.glyphs.matrix"] = app->LoadResource(204);
    app->asset_cache["Nobile.ttf,32,255,255,255,0.0000.png"]           = app->LoadResource(205);
  }
#endif
  if (app->Init()) return -1;
  app->fonts->atlas_engine.get(app->fonts.get())->Init(FontDesc("ChessPieces1", "", 0, Color::white, Color::clear, 0, false));
  app->scheduler.AddMainWaitKeyboard(app->focused);
  app->scheduler.AddMainWaitMouse(app->focused);
  app->StartNewWindow(app->focused);

  ChessView *chess_view = app->focused->GetOwnView<ChessView>(0);
  auto seek_command = &FLAGS_seek_command;

  // app->asset.Add(app, name,  texture,      scale, translate, rotate, geometry, hull,    0, 0);
  app->asset.Add(app, "board1", "board1.png", 0,     0,         0,      nullptr,  nullptr, 0, 0);
  app->asset.Load();

  // app->soundasset.Add(app, name,   filename,      ringbuf, channels, sample_rate, seconds );
  app->soundasset.Add(app, "start",   "start.wav",   nullptr, 0,        0,           0       );
  app->soundasset.Add(app, "move",    "move.wav",    nullptr, 0,        0,           0       );
  app->soundasset.Add(app, "capture", "capture.wav", nullptr, 0,        0,           0       );
  app->soundasset.Add(app, "win",     "win.wav",     nullptr, 0,        0,           0       );
  app->soundasset.Add(app, "lose",    "lose.wav",    nullptr, 0,        0,           0       );
  app->soundasset.Add(app, "illegal", "illegal.wav", nullptr, 0,        0,           0       );
  app->soundasset.Load();

  app->askseek = app->toolkit->CreateAlert(app->focused, AlertItemVec{
    { "style", "textinput" }, { "Seek Game", "Edit seek game criteria" }, { "Cancel", },
    { "Continue", "", bind([=](const string &a){ chess_view->Send("seek " + (*seek_command = a)); }, _1)}
  });

  app->askresign = app->toolkit->CreateAlert(app->focused, AlertItemVec{
    { "style", "confirm" }, { "Confirm resign", "Do you wish to resign?" }, { "No" },
    { "Yes", "", bind([=](){ chess_view->Send("resign"); })}
  });

#ifndef LFL_MOBILE
  app->editmenu = app->toolkit->CreateEditMenu(app->focused, MenuItemVec{
    MenuItem{ "u", "Undo pre-move",         bind(&ChessView::UndoPremove, chess_view, app->focused)},
    MenuItem{ "",  "Copy PGN to clipboard", bind(&ChessView::CopyPGNToClipboard, chess_view)}
  });
  app->viewmenu = app->toolkit->CreateMenu(app->focused, "View", MenuItemVec{
    MenuItem{ "f",       "Flip board", bind(&ChessView::FlipBoard, chess_view, app->focused)},
    MenuItem{ "<left>",  "Previous move" },
    MenuItem{ "<right>", "Next move" },
    MenuItem{ "<up>",    "Scroll up" },    
    MenuItem{ "<down>",  "Scroll down" }
  });
  {
    MenuItemVec gamemenu{
      MenuItem{ "s", "Seek",       bind([=](){ app->askseek->Show(*seek_command); })},
      MenuItem{ "d", "Offer Draw", bind([=](){ chess_view->Send("draw"); })},
      MenuItem{ "r", "Resign",     bind([=](){ app->askresign->Show(""); })}
    };
    if (FLAGS_engine.size()) {
      gamemenu.push_back(MenuItem{"", "Engine play white", bind(&ChessView::StartEngine, chess_view, false) });
      gamemenu.push_back(MenuItem{"", "Engine play black", bind(&ChessView::StartEngine, chess_view, true) });
    }
    app->gamemenu = app->toolkit->CreateMenu(app->focused, "Game", move(gamemenu));
  }
#else
  app->maintoolbar = SystemToolkit::CreateToolbar("", MenuItemVec{ 
    MenuItem{ "\U000025C0", "", bind(&ChessGUI::WalkHistory, chess_gui, true) },
    MenuItem{ "\U000025B6", "", bind(&ChessGUI::WalkHistory, chess_gui, false) },
    MenuItem{ "seek",       "", bind([=](){ app->askseek->Show(*seek_command); }) },
    MenuItem{ "resign",     "", bind([=](){ app->askresign->Show(""); }) },
    MenuItem{ "draw",       "", bind([=](){ chess_gui->Send("draw"); }) },
    MenuItem{ "flip",       "", bind(&ChessGUI::FlipBoard,   chess_gui, app->focused) },
    MenuItem{ "undo",       "", bind(&ChessGUI::UndoPremove, chess_gui, app->focused) }
  });
  app->maintoolbar->Show(true);
#endif

  chess_view->Open(FLAGS_connect);
  return app->Main();
}
