import subprocess, threading, sys, os, pygame

SQ = 80
W, H = SQ * 8, SQ * 8 + 40
LIGHT = (210, 187, 151)
DARK = (181, 136, 99)
HIGHLIGHT = (186, 202, 68, 150)
FPS = 30

PIECE_UNICODE = {
    "♟": ("P", True), "♞": ("N", True), "♝": ("B", True),
    "♜": ("R", True), "♛": ("Q", True), "♚": ("K", True),
    "♙": ("P", False), "♘": ("N", False), "♗": ("B", False),
    "♖": ("R", False), "♕": ("Q", False), "♔": ("K", False),
}

PIECE_CHARS = {
    ("P", True): "♙", ("N", True): "♘", ("B", True): "♗",
    ("R", True): "♖", ("Q", True): "♕", ("K", True): "♔",
    ("P", False): "♟", ("N", False): "♞", ("B", False): "♝",
    ("R", False): "♜", ("Q", False): "♕", ("K", False): "♔",
}

def sq_name(file, rank):
    return chr(ord('a') + file) + str(rank + 1)

class Engine:
    def __init__(self, exe):
        self.proc = subprocess.Popen(
            [exe], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, text=True, bufsize=1,
            creationflags=subprocess.CREATE_NO_WINDOW if sys.platform == "win32" else 0
        )
        self.lines = []
        self.lock = threading.Lock()
        self.board = [[None]*8 for _ in range(8)]
        self.turn = "White"
        self.legal_moves = set()
        self.status = ""
        t = threading.Thread(target=self._read, daemon=True)
        t.start()

    def _read(self):
        for line in self.proc.stdout:
            line = line.rstrip('\n').rstrip('\r')
            with self.lock:
                self.lines.append(line)

    def send(self, move_str):
        self.proc.stdin.write(move_str + "\n")
        self.proc.stdin.flush()

    def poll(self):
        with self.lock:
            buf = list(self.lines)
            self.lines.clear()
        if not buf:
            return
        self._got_turn = False
        self._got_moves = False
        for line in buf:
            self._parse(line)
        if self._got_turn and not self._got_moves:
            self.legal_moves.clear()
            self.status = "Checkmate"

    def _parse(self, line):
        line_stripped = line.strip()
        # print("DEBUG LINE:", repr(line_stripped))
        if line_stripped.startswith(("1 |", "2 |", "3 |", "4 |", "5 |", "6 |", "7 |", "8 |")):
            rank = int(line_stripped[0]) - 1
            parts = line_stripped.split("|")[1:]
            for file_idx, part in enumerate(parts):
                if file_idx >= 8:
                    break
                ch = part.strip()
                if ch and ch in PIECE_UNICODE:
                    p, is_white = PIECE_UNICODE[ch]
                    self.board[rank][file_idx] = (p, is_white)
                else:
                    self.board[rank][file_idx] = None
        elif "to move" in line_stripped.lower() or line_stripped.startswith("Turn:"):
            self._got_turn = True
            if "White" in line_stripped:
                self.turn = "White"
            elif "Black" in line_stripped:
                self.turn = "Black"
        elif "|" in line_stripped and not line_stripped.startswith("+"):
            self._got_moves = True
            moves = line_stripped.split("|")
            self.legal_moves.clear()
            for m in moves:
                m = m.strip()
                if not m: continue
                parts = m.split()
                if len(parts) == 1:
                    m_str = parts[0]
                    if len(m_str) in (4, 5):
                        self.legal_moves.add(m_str)
                elif len(parts) == 2 and len(parts[0]) == 2 and len(parts[1]) == 2:
                    self.legal_moves.add(parts[0] + parts[1])
            if not self.legal_moves:
                self.status = "Checkmate"
        if "wins by checkmate" in line_stripped:
            self.status = line_stripped
        elif "drawn by stalemate" in line_stripped:
            self.status = line_stripped
        elif "Illegal move" in line_stripped:
            self.status = "Illegal move"

def main():
    exe = os.path.join(os.path.dirname(os.path.abspath(__file__)), "game.exe")
    if not os.path.exists(exe):
        print(f"Cannot find {exe}")
        return

    engine = Engine(exe)

    pygame.init()
    screen = pygame.display.set_mode((W, H))
    pygame.display.set_caption("Chess Debug GUI")
    clock = pygame.time.Clock()

    try:
        piece_font = pygame.font.SysFont("segoeuisymbol", SQ - 14)
    except Exception:
        piece_font = pygame.font.SysFont(None, SQ - 14)
    info_font = pygame.font.SysFont("consolas", 18)

    selected = None
    legal_for_selected = []

    running = True
    while running:
        engine.poll()

        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                running = False
            elif ev.type == pygame.MOUSEBUTTONDOWN and ev.button == 1:
                mx, my = ev.pos
                if my < SQ * 8:
                    file = mx // SQ
                    rank = 7 - (my // SQ)
                    if selected is None:
                        if engine.legal_moves:
                            selected = (file, rank)
                            sq_from = sq_name(file, rank)
                            legal_for_selected = [m for m in engine.legal_moves if m[:2] == sq_from]
                    else:
                        sq_from = sq_name(selected[0], selected[1])
                        sq_to = sq_name(file, rank)
                        base_move = sq_from + sq_to
                        
                        promotions = [m[-1] for m in engine.legal_moves if m.startswith(base_move) and len(m) == 5]
                        
                        if promotions:
                            promoted = False
                            promo_rects = []
                            promo_w = 4 * SQ
                            promo_x = (W - promo_w) // 2
                            promo_y = (H - SQ) // 2
                            
                            while not promoted:
                                for ev2 in pygame.event.get():
                                    if ev2.type == pygame.QUIT:
                                        pygame.quit()
                                        return
                                    elif ev2.type == pygame.MOUSEBUTTONDOWN and ev2.button == 1:
                                        mx2, my2 = ev2.pos
                                        for pr, p_char in promo_rects:
                                            if pr.collidepoint(mx2, my2):
                                                engine.send(base_move + p_char)
                                                engine.status = ""
                                                promoted = True
                                                break
                                
                                screen.fill((30, 30, 30))
                                for r in range(8):
                                    for f in range(8):
                                        pygame.draw.rect(screen, LIGHT if (r + f) % 2 == 0 else DARK, (f * SQ, (7 - r) * SQ, SQ, SQ))
                                        piece = engine.board[r][f]
                                        if piece:
                                            p, is_white = piece
                                            ch = PIECE_CHARS.get((p, is_white), "?")
                                            color = (255, 255, 255) if is_white else (0, 0, 0)
                                            txt = piece_font.render(ch, True, color)
                                            screen.blit(txt, (f * SQ + (SQ - txt.get_width()) // 2, (7 - r) * SQ + (SQ - txt.get_height()) // 2))
                                            
                                s = pygame.Surface((W, H))
                                s.set_alpha(128)
                                s.fill((0, 0, 0))
                                screen.blit(s, (0,0))
                                
                                pygame.draw.rect(screen, (200, 200, 200), (promo_x, promo_y, promo_w, SQ))
                                pygame.draw.rect(screen, (50, 50, 50), (promo_x, promo_y, promo_w, SQ), 3)
                                
                                choices = ["Q", "R", "B", "N"]
                                is_white_promo = engine.turn == "White"
                                promo_rects = []
                                
                                for i, p_char in enumerate(choices):
                                    rx = promo_x + i * SQ
                                    r = pygame.Rect(rx, promo_y, SQ, SQ)
                                    promo_rects.append((r, p_char))
                                    pygame.draw.rect(screen, (150, 150, 150), r, 1)
                                    
                                    ch = PIECE_CHARS.get((p_char, is_white_promo), "?")
                                    color = (255, 255, 255) if is_white_promo else (0, 0, 0)
                                    txt = piece_font.render(ch, True, color)
                                    screen.blit(txt, (rx + (SQ - txt.get_width()) // 2, promo_y + (SQ - txt.get_height()) // 2))
                                    
                                pygame.display.flip()
                                clock.tick(FPS)
                                
                            selected = None
                            legal_for_selected = []
                            
                        elif base_move in engine.legal_moves:
                            engine.send(base_move)
                            engine.status = ""
                            selected = None
                            legal_for_selected = []
                        else:
                            sq_new_from = sq_name(file, rank)
                            new_legal = [m for m in engine.legal_moves if m[:2] == sq_new_from]
                            if len(new_legal) > 0:
                                selected = (file, rank)
                                legal_for_selected = new_legal
                            else:
                                selected = None
                                legal_for_selected = []
            elif ev.type == pygame.KEYDOWN:
                if ev.key == pygame.K_ESCAPE:
                    selected = None
                    legal_for_selected = []
                elif ev.key == pygame.K_r:
                    engine.send("reset")
                    engine.status = ""
                    selected = None
                    legal_for_selected = []

        screen.fill((30, 30, 30))

        for r in range(8):
            for f in range(8):
                x = f * SQ
                y = (7 - r) * SQ
                color = LIGHT if (r + f) % 2 == 0 else DARK
                pygame.draw.rect(screen, color, (x, y, SQ, SQ))

        if selected:
            sx = selected[0] * SQ
            sy = (7 - selected[1]) * SQ
            hl = pygame.Surface((SQ, SQ), pygame.SRCALPHA)
            hl.fill((255, 255, 100, 100))
            screen.blit(hl, (sx, sy))
            for m in legal_for_selected:
                tf, tr = ord(m[2]) - ord('a'), int(m[3]) - 1
                cx = tf * SQ + SQ // 2
                cy = (7 - tr) * SQ + SQ // 2
                pygame.draw.circle(screen, (100, 100, 100, 180), (cx, cy), SQ // 6)

        for r in range(8):
            for f in range(8):
                piece = engine.board[r][f]
                if piece:
                    p, is_white = piece
                    ch = PIECE_CHARS.get((p, is_white), "?")
                    color = (255, 255, 255) if is_white else (0, 0, 0)
                    txt = piece_font.render(ch, True, color)
                    x = f * SQ + (SQ - txt.get_width()) // 2
                    y = (7 - r) * SQ + (SQ - txt.get_height()) // 2
                    screen.blit(txt, (x, y))

        if not engine.legal_moves and any(engine.board[r][f] for r in range(8) for f in range(8)):
            overlay = pygame.Surface((W, SQ * 8), pygame.SRCALPHA)
            overlay.fill((0, 0, 0, 160))
            screen.blit(overlay, (0, 0))

            big_font = pygame.font.SysFont("consolas", 72, bold=True)
            small_font = pygame.font.SysFont("consolas", 32)

            title = big_font.render("CHECKMATE", True, (255, 70, 70))
            screen.blit(title, ((W - title.get_width()) // 2, SQ * 3 - 30))

            winner = "Black" if engine.turn == "White" else "White"
            subtitle = small_font.render(f"{winner} wins!", True, (220, 220, 220))
            screen.blit(subtitle, ((W - subtitle.get_width()) // 2, SQ * 3 + 50))

            hint = info_font.render("Press R to play again", True, (180, 180, 180))
            screen.blit(hint, ((W - hint.get_width()) // 2, SQ * 3 + 100))

        bar_y = SQ * 8
        pygame.draw.rect(screen, (50, 50, 50), (0, bar_y, W, 40))
        status_text = engine.status if engine.status else f"{engine.turn} to move"
        txt = info_font.render(status_text, True, (220, 220, 220))
        screen.blit(txt, (10, bar_y + 10))

        pygame.display.flip()
        clock.tick(FPS)

    engine.send("quit")
    engine.proc.terminate()
    pygame.quit()

if __name__ == "__main__":
    main()
