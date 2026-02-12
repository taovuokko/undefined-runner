#include "raylib.h"
#include <string>

// Hardcoded values
const int width{800};
const int height{400};
const std::string GameName("Undefined Runner");
const int FPS{60};
const int rec_width{50};
const int rec_height{80};
float jumpVel{-600.0f};
int frame{};
const float updateTime = 1.0 / 12.0;

// nebula X velocity (pixels/second)
int NebVel{-500};

struct InputState {
  bool jumpPressed;
};

struct Physics {
  const float gravity{1000.0f};
};

struct Sprite {

  Texture2D texture;
  Texture2D nebula;
  Rectangle scarfyRec;
  Rectangle nebRec;
  Vector2 nebPos;
  Vector2 scarfyPos;
  float runningTime;
  bool onGround;
};

InputState ReadInput() {
  InputState input{};
  input.jumpPressed = IsKeyPressed(KEY_SPACE);
  return input;
}

Sprite LoadScarfy() {
  Sprite s{};
  s.onGround = true;
  s.texture = LoadTexture("textures/scarfy.png");
  s.scarfyRec = {0, 0, s.texture.width / 6.0f, (float)s.texture.height};
  s.scarfyPos = {width / 2.0f, height - s.scarfyRec.height};
  return s;
}

Sprite LoadNebula() {
  Sprite n{};
  n.nebula = LoadTexture("textures/nebula.png");
  n.nebRec = {0.0f, 0.0f, n.nebula.width / 8.0f, float(n.nebula.height) / 8};
  n.nebPos = {width, (float)height - n.nebRec.height};
  return n;
}

void DrawScarfy(const Sprite &s) {
  DrawTextureRec(s.texture, s.scarfyRec, s.scarfyPos, WHITE);
}

void DrawNebula(const Sprite &n) {
  DrawTextureRec(n.nebula, n.nebRec, n.nebPos, WHITE);
}
void AnimateScrafy(Sprite &s, float dT) {
  s.runningTime += dT;
  if (s.onGround && s.runningTime >= updateTime) {
    s.runningTime = 0.0f;
    s.scarfyRec.x = frame * s.scarfyRec.width;
    frame++;
    if (frame > 5) {
      frame = 0;
    }
  }
}

void UpdatePlayer(Sprite &player, float &velocity, const InputState &input,
                  const Physics &physics, float dT) {

  float groundY = height - player.scarfyRec.height;

  if (input.jumpPressed && player.onGround) {
    velocity = jumpVel;
  }

  velocity += physics.gravity * dT;
  player.scarfyPos.y += velocity * dT;

  if (player.scarfyPos.y > groundY) {
    player.scarfyPos.y = groundY;
    velocity = 0.0f;
  }
  player.onGround = (player.scarfyPos.y >= groundY);
}

void UpdateNebula(Sprite &nebula, float dT) { nebula.nebPos.x += NebVel * dT; }

int main(void) {
  Physics physics;
  float velocity{0};

  SetTargetFPS(FPS);
  InitWindow(width, height, GameName.c_str());
  SetExitKey(KEY_Q);
  Sprite scarfy = LoadScarfy();
  Sprite nebula = LoadNebula();
  // Gameloop
  while (!WindowShouldClose()) {
    float dT = GetFrameTime();
    InputState input = ReadInput();
    UpdatePlayer(scarfy, velocity, input, physics, dT);
    UpdateNebula(nebula, dT);
    BeginDrawing();
    ClearBackground(WHITE);
    DrawScarfy(scarfy);
    DrawNebula(nebula);
    AnimateScrafy(scarfy, dT);

    EndDrawing();
  }
  UnloadTexture(scarfy.texture);
  UnloadTexture(nebula.nebula);
  CloseWindow();
}
