import pygame
import sys

pygame.init()

WINDOW_WIDTH = 800
WINDOW_HEIGHT = 900  # Increased height for input box
BOARD_SIZE = 600
SQUARE_SIZE = BOARD_SIZE // 8
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
HIGHLIGHT = (100, 150, 100)
TEXT_COLOR = (50, 50, 50)
BUTTON_COLOR = (100, 150, 200)
BUTTON_HOVER_COLOR = (80, 130, 180)
INPUT_BOX_COLOR = (240, 240, 240)

screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Bitboard Visualizer - Manual Input")

class Button:
    def __init__(self, x, y, width, height, text):
        self.rect = pygame.Rect(x, y, width, height)
        self.text = text
        self.font = pygame.font.SysFont('Arial', 24)
        self.is_hovered = False

    def draw(self):
        color = BUTTON_HOVER_COLOR if self.is_hovered else BUTTON_COLOR
        pygame.draw.rect(screen, color, self.rect)
        pygame.draw.rect(screen, BLACK, self.rect, 2)
        text_surface = self.font.render(self.text, True, WHITE)
        text_rect = text_surface.get_rect(center=self.rect.center)
        screen.blit(text_surface, text_rect)

    def handle_event(self, event):
        if event.type == pygame.MOUSEMOTION:
            self.is_hovered = self.rect.collidepoint(event.pos)
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1 and self.rect.collidepoint(event.pos):
                return True
        return False

class ChessboardVisualizer:
    def __init__(self):
        self.board_state = 0
        self.input_text = ""
        self.font = pygame.font.SysFont('Arial', 24)
        self.board_offset_x = (WINDOW_WIDTH - BOARD_SIZE) // 2
        self.board_offset_y = 50
        self.reset_button = Button(
            (WINDOW_WIDTH - 150) // 2,
            self.board_offset_y + BOARD_SIZE + 180,
            150, 40, "Reset"
        )
    
    def get_square_from_pos(self, pos):
        x, y = pos
        x -= self.board_offset_x
        y -= self.board_offset_y
        if not (0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE):
            return None
        file = x // SQUARE_SIZE
        rank = 7 - (y // SQUARE_SIZE)
        return rank * 8 + file
    
    def toggle_square(self, square_index):
        if square_index is not None:
            self.board_state ^= (1 << square_index)
            self.input_text = str(self.board_state) # Sync input box
    
    def reset(self):
        self.board_state = 0
        self.input_text = "0"
    
    def handle_keyboard(self, event):
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_BACKSPACE:
                self.input_text = self.input_text[:-1]
            elif event.unicode.isdigit():
                self.input_text += event.unicode
            
            # Update board state from string
            if self.input_text:
                try:
                    self.board_state = int(self.input_text)
                except ValueError:
                    pass
            else:
                self.board_state = 0

    def draw(self):
        screen.fill(WHITE)
        
        # Draw board squares
        for rank in range(8):
            for file in range(8):
                x = self.board_offset_x + file * SQUARE_SIZE
                y = self.board_offset_y + (7 - rank) * SQUARE_SIZE
                base_color = WHITE if (rank + file) % 2 == 0 else (220, 220, 220)
                bit_pos = rank * 8 + file
                color = HIGHLIGHT if (self.board_state & (1 << bit_pos)) else base_color
                pygame.draw.rect(screen, color, (x, y, SQUARE_SIZE, SQUARE_SIZE))
                
                # Coordinates
                if rank == 0:
                    lbl = self.font.render(chr(file + 97), True, BLACK)
                    screen.blit(lbl, (x + 30, self.board_offset_y + BOARD_SIZE + 5))
                if file == 0:
                    lbl = self.font.render(str(rank + 1), True, BLACK)
                    screen.blit(lbl, (self.board_offset_x - 25, y + 15))

        # UI Info
        text_y = self.board_offset_y + BOARD_SIZE + 40
        hex_text = self.font.render(f"Hex: 0x{self.board_state:016X}", True, TEXT_COLOR)
        screen.blit(hex_text, (self.board_offset_x, text_y))

        # Input Box Label
        input_lbl = self.font.render("Type Integer:", True, BLACK)
        screen.blit(input_lbl, (self.board_offset_x, text_y + 40))

        # Input Box
        input_rect = pygame.Rect(self.board_offset_x, text_y + 70, 400, 40)
        pygame.draw.rect(screen, INPUT_BOX_COLOR, input_rect)
        pygame.draw.rect(screen, BLACK, input_rect, 2)
        txt_surface = self.font.render(self.input_text, True, BLACK)
        screen.blit(txt_surface, (input_rect.x + 10, input_rect.y + 5))

        self.reset_button.draw()
        pygame.display.flip()

def main():
    visualizer = ChessboardVisualizer()
    clock = pygame.time.Clock()
    
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit(); sys.exit()
            
            visualizer.handle_keyboard(event)
            
            if event.type == pygame.MOUSEBUTTONDOWN:
                if visualizer.reset_button.handle_event(event):
                    visualizer.reset()
                else:
                    square = visualizer.get_square_from_pos(event.pos)
                    visualizer.toggle_square(square)
            else:
                visualizer.reset_button.handle_event(event)
        
        visualizer.draw()
        clock.tick(60)

if __name__ == "__main__":
    main()