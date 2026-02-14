#include "raylib.h"
#include <algorithm>
#include <ctime>
#include <string>
#include <vector>

// Hardcoded values
const int width{800};
const int height{400};
const std::string GameName("Undefined Runner");
const int FPS{60};
const int rec_width{50};
const int rec_height{80};
const float updateTime = 1.0 / 12.0;
const float nebUpdateTime = 1.0f / 12.0;

struct InputState {
  bool jumpPressed;
};

struct Physics {
  const float gravity{1000.0f};
};

struct Textures {
  Texture2D scarfy;
  Texture2D nebula;
};

struct Player {

  Rectangle rec;
  Vector2 pos;
  float runningTime;
  int frame;
  float jumpVel;
  float velocity;
  bool onGround;
};

struct Nebula {

  Rectangle nebRec;
  Vector2 nebPos;
  float nebRunningTime;
  float velocityX;
  int nebFrame;
  Color tint;
};

struct GameState {
  Player player;
  std::vector<Nebula> nebulae;
  float spawnTimer{};
  const float spawnInterval{2.0f};
};

InputState ReadInput() {
  InputState input{};
  input.jumpPressed = IsKeyPressed(KEY_SPACE);
  return input;
}
Textures LoadTextures() {
  Textures t{};
  t.scarfy = LoadTexture("textures/scarfy.png");
  t.nebula = LoadTexture("textures/nebula.png");
  return t;
}

void UpdatePlayer(Player &player, const InputState &input,
                  const Physics &physics, float dT);

void UpdateNebula(Nebula &n, float dT);

Player LoadScarfy(const Texture2D &scaTex) {
  Player s{};
  s.jumpVel = -600.0f;
  s.onGround = true;
  s.rec = {0, 0, scaTex.width / 6.0f, (float)scaTex.height};
  s.pos = {width / 2.0f, height - s.rec.height};
  return s;
}

Nebula LoadNebula(const Texture2D &nebTex) {
  Nebula n{};
  n.tint = WHITE;
  n.velocityX = -500.0f;
  n.nebRec = {0.0f, 0.0f, nebTex.width / 8.0f, float(nebTex.height) / 8};
  n.nebPos = {width, (float)height - n.nebRec.height};
  return n;
}

void DrawScarfy(const Player &s, const Texture2D &tex) {
  DrawTextureRec(tex, s.rec, s.pos, WHITE);
}

void DrawNebula(const Nebula &n, const Texture2D &tex) {
  DrawTextureRec(tex, n.nebRec, n.nebPos, n.tint);
}
void AnimateScrafy(Player &s, float dT) {

  s.runningTime += dT;
  if (s.onGround && s.runningTime >= updateTime) {
    s.runningTime = 0.0f;
    s.rec.x = s.frame * s.rec.width;
    s.frame++;
    if (s.frame > 5) {
      s.frame = 0;
    }
  }
}

void AnimateNebula(Nebula &n, float dT) {

  n.nebRunningTime += dT;
  if (n.nebRunningTime >= nebUpdateTime) {
    n.nebRunningTime = 0.0f;
    n.nebRec.x = n.nebFrame * n.nebRec.width;
    n.nebFrame++;
    if (n.nebFrame > 5) {
      n.nebFrame = 0;
    }
  }
}

void UpdateGame(GameState &state, const InputState &input,
                const Physics &physics, float dT, const Texture2D &nebTex) {
  state.spawnTimer += dT;
  if (state.spawnTimer >= state.spawnInterval) {
    Nebula newNebula = LoadNebula(nebTex);
    newNebula.velocityX = -400.0f - (rand() % 200);

    int colorIndex = rand() % 4;

    if (colorIndex == 0)
      newNebula.tint = PURPLE;
    if (colorIndex == 1)
      newNebula.tint = RED;
    if (colorIndex == 2)
      newNebula.tint = WHITE;
    if (colorIndex == 3)
      newNebula.tint = BLUE;

    state.nebulae.push_back(newNebula);
    state.spawnTimer = 0.0f;
  }

  UpdatePlayer(state.player, input, physics, dT);

  for (auto &n : state.nebulae) {
    UpdateNebula(n, dT);
  }

  state.nebulae.erase(std::remove_if(state.nebulae.begin(), state.nebulae.end(),
                                     [](const Nebula &n) {
                                       return n.nebPos.x < -n.nebRec.width;
                                     }),
                      state.nebulae.end());
}

void UpdatePlayer(Player &player, const InputState &input,
                  const Physics &physics, float dT) {

  float groundY = height - player.rec.height;

  if (input.jumpPressed && player.onGround) {
    player.velocity = player.jumpVel;
  }

  player.velocity += physics.gravity * dT;
  player.pos.y += player.velocity * dT;

  if (player.pos.y > groundY) {

    player.pos.y = groundY;
    player.velocity = 0.0f;
  }
  player.onGround = (player.pos.y >= groundY);
}

void UpdateNebula(Nebula &n, float dT) { n.nebPos.x += n.velocityX * dT; }

int main(void) {
  Physics physics;
  GameState state;
  srand(time((nullptr)));

  SetTargetFPS(FPS);
  InitWindow(width, height, GameName.c_str());
  SetExitKey(KEY_Q);
  Textures textures = LoadTextures();
  state.player = LoadScarfy(textures.scarfy);
  // Gameloop
  while (!WindowShouldClose()) {
    float dT = GetFrameTime();
    InputState input = ReadInput();
    UpdateGame(state, input, physics, dT, textures.nebula);

    BeginDrawing();
    ClearBackground(WHITE);
    DrawScarfy(state.player, textures.scarfy);
    for (auto &n : state.nebulae) {
      DrawNebula(n, textures.nebula);
      AnimateNebula(n, dT);
    }
    AnimateScrafy(state.player, dT);
    EndDrawing();
  }
  UnloadTexture(textures.scarfy);
  UnloadTexture(textures.nebula);
  CloseWindow();
}
