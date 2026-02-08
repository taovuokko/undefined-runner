#include "raylib.h"
#include <string>

// Hardcoded values
const int width{800};
const int height{400};
const std::string GameName("Undefined Runner");
const int FPS{60};
const int rec_width{50};
const int rec_height{80};
int jumpVel{-80};

struct InputState {
  bool jumpPressed;
};

struct Physics {
  const int gravity{10};
};

InputState ReadInput();
void UpdatePlayer(int &posY, int &velocity, const InputState &input,
                  const Physics &physics);

int main(void) {
  Physics physics;

  int posY{height - rec_height};
  int velocity{0};

  SetTargetFPS(FPS);
  InitWindow(width, height, GameName.c_str());
  SetExitKey(KEY_Q);

  // Gameloop
  while (!WindowShouldClose()) {
    InputState input = ReadInput();
    UpdatePlayer(posY, velocity, input, physics);
    BeginDrawing();
    ClearBackground(WHITE);

    DrawRectangle(width / 2, posY, rec_width, rec_height, BLUE);

    EndDrawing();
  }
  CloseWindow();
}

InputState ReadInput() {
  InputState input{};
  input.jumpPressed = IsKeyPressed(KEY_SPACE);
  return input;
}

void UpdatePlayer(int &posY, int &velocity, const InputState &input,
                  const Physics &physics) {

  bool onGround = (posY >= height - rec_height);

  if (input.jumpPressed && onGround) {
    velocity = jumpVel;
  }

  velocity += physics.gravity;
  posY += velocity;

  if (posY > height - rec_height) {
    posY = height - rec_height;
    velocity = 0;
  }
}
