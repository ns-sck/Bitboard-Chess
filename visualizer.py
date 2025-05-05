import pygame
import sys

pygame.init()

WINDOW_WIDTH = 600
WINDOW_HEIGHT = 500
BOARD_SIZE = 400
SQUARE_SIZE = BOARD_SIZE // 8
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
HIGHLIGHT = (100, 100, 100)
BORDER = (150, 150, 150)
TEXT_COLOR = (50, 50, 50)
BUTTON_COLOR = (100, 150, 200)
BUTTON_HOVER_COLOR = (80, 130, 180)

screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Binary Chessboard Visualizer")

class Button:
    def __init__(self, x, y, width, height, text):
        self.rect = pygame.Rect(x, y, width, height)
        self.text = text
        self.font = pygame.font.SysFont('Arial', 20)
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
        self.font = pygame.font.SysFont('Arial', 20)
        self.board_offset_x = (WINDOW_WIDTH - BOARD_SIZE) // 2
        self.board_offset_y = 20
        self.reset_button = Button(
            WINDOW_WIDTH - 120, 
            WINDOW_HEIGHT - 60, 
            100, 40, "Reset"
        )
    
    def get_square_from_pos(self, pos):
        # Convert mouse position to board coordinates
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
    
    def reset(self):
        self.board_state = 0
    
    def draw(self):
        screen.fill(WHITE)
        
        for rank in range(8):
            for file in range(8):
                x = self.board_offset_x + file * SQUARE_SIZE
                y = self.board_offset_y + (7 - rank) * SQUARE_SIZE
                
                base_color = WHITE if (rank + file) % 2 == 0 else (200, 200, 200)
                
                bit_pos = rank * 8 + file
                
                color = HIGHLIGHT if (self.board_state & (1 << bit_pos)) else base_color
                
                pygame.draw.rect(screen, color, (x, y, SQUARE_SIZE, SQUARE_SIZE))
                
                if rank == 7:
                    label = self.font.render(chr(file + 97), True, BLACK)
                    screen.blit(label, (x + SQUARE_SIZE - 15, 
                              self.board_offset_y + BOARD_SIZE - 15))
                
                if file == 0:
                    label = self.font.render(str(rank + 1), True, BLACK)
                    screen.blit(label, (self.board_offset_x - 20, y + 5))
        
        pygame.draw.rect(screen, BLACK, 
                        (self.board_offset_x, self.board_offset_y, 
                         BOARD_SIZE, BOARD_SIZE), 2)
        
        hex_text = self.font.render(f"Hex: 0x{self.board_state:016X}", True, TEXT_COLOR)
        dec_text = self.font.render(f"Dec: {self.board_state}", True, TEXT_COLOR)
        
        text_y = self.board_offset_y + BOARD_SIZE + 20
        screen.blit(hex_text, (self.board_offset_x, text_y))
        screen.blit(dec_text, (self.board_offset_x, text_y + 30))
        
        self.reset_button.draw()
        
        pygame.display.flip()

def main():
    visualizer = ChessboardVisualizer()
    running = True
    clock = pygame.time.Clock()
    
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:  # Left click
                    if visualizer.reset_button.handle_event(event):
                        visualizer.reset()
                    else:
                        square = visualizer.get_square_from_pos(event.pos)
                        visualizer.toggle_square(square)
            else:
                visualizer.reset_button.handle_event(event)
        
        visualizer.draw()
        clock.tick(60)
    
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()