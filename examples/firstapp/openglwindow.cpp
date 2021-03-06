#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

struct EstadoLinha {
  char* colA = NULL;
  char* colB = NULL;
  char* colC = NULL;
};

char PLAYER_X{'X'};
char PLAYER_0{'0'};

char* player{&PLAYER_X};
static EstadoLinha* linha1 = (EstadoLinha*)malloc(sizeof(EstadoLinha));
static EstadoLinha* linha2 = (EstadoLinha*)malloc(sizeof(EstadoLinha));
static EstadoLinha* linha3 = (EstadoLinha*)malloc(sizeof(EstadoLinha));
static bool ENDGAME = false;

void iniciaLinha(EstadoLinha* linha);
void criaButton(int h, char** texto);
void restartButton(int h);
void trocaPlayer();
void limpaLinha(EstadoLinha* linha);
void drawButtons(int h, EstadoLinha* a, EstadoLinha* b, EstadoLinha* c);
void drawThreeButtons(int h, EstadoLinha* linha);
char pickWinner();
bool isEndgame();
bool linhaCheia(EstadoLinha* a);
bool campoPreenchido(char* conteudo);
bool isWinner(char c);

void OpenGLWindow::initializeGL() {
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void OpenGLWindow::paintGL() {
  // Set the clear color
  glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
               m_clearColor[3]);
  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  // abcg::OpenGLWindow::paintUI();
  // Our own ImGui widgets go below
  iniciaLinha(linha1);
  iniciaLinha(linha2);
  iniciaLinha(linha3);
  {
    auto windowSettings{getWindowSettings()};
    float alturaAtual = ImGui::GetWindowHeight();
    int h = alturaAtual > 60 ? alturaAtual - 60 : alturaAtual;

    ImGui::SetNextWindowSize(ImVec2(((float)windowSettings.width / 1.2),
                                    ((float)windowSettings.height / 1.2)));
    // Window begin
    ImGui::Begin("Tic-Tac-Toe!");

    if (!isEndgame()) {
      // Static text
      ImGui::Text("Vez do jogador %c\n", *player);
    } else {
      ENDGAME = true;
      char ganhador = pickWinner();
      if (ganhador == ' ') {
        ImGui::Text("Deu velha");
      } else {
        ImGui::Text("Fim de jogo o jogador %c ganhou!", ganhador);
      }
    }
    drawButtons(h, linha1, linha2, linha3);
    restartButton(alturaAtual - h - 10);

    // Window end
    ImGui::End();
  }
}

bool isEndgame() {
  return (linhaCheia(linha1) && linhaCheia(linha2) && linhaCheia(linha3)) ||
         pickWinner() == PLAYER_X || pickWinner() == PLAYER_0;
}

bool linhaCheia(EstadoLinha* linha) {
  return campoPreenchido(linha->colA) && campoPreenchido(linha->colB) &&
         campoPreenchido(linha->colC);
}

bool campoPreenchido(char* conteudo) {
  return *(conteudo) == PLAYER_X || *(conteudo) == PLAYER_0;
}

char pickWinner() {
  if (isWinner(PLAYER_X)) {
    return PLAYER_X;
  } else if (isWinner(PLAYER_0)) {
    return PLAYER_0;
  }
  return ' ';
}

bool isWinner(char c) {
  return (*(linha1->colA) == c && *(linha1->colB) == c &&
          *(linha1->colC) == c) ||
         (*(linha2->colA) == c && *(linha2->colB) == c &&
          *(linha2->colC) == c) ||
         (*(linha3->colA) == c && *(linha3->colB) == c &&
          *(linha3->colC) == c) ||
         (*(linha1->colA) == c && *(linha2->colA) == c &&
          *(linha3->colA) == c) ||
         (*(linha1->colB) == c && *(linha2->colB) == c &&
          *(linha3->colB) == c) ||
         (*(linha1->colC) == c && *(linha2->colC) == c &&
          *(linha3->colC) == c) ||
         (*(linha1->colC) == c && *(linha2->colB) == c &&
          *(linha3->colA) == c) ||
         (*(linha1->colA) == c && *(linha2->colB) == c && *(linha3->colC) == c);
}

void iniciaLinha(EstadoLinha* linha) {
  if (linha->colA == NULL) {
    linha->colA = (char*)malloc(sizeof(char*));
    *(linha->colA) = '\0';
    std::printf("batata A\n\n");
  }
  if (linha->colB == NULL) {
    linha->colB = (char*)malloc(sizeof(char*));
    *(linha->colB) = '\0';
    std::printf("batata B\n\n");
  }
  if (linha->colC == NULL) {
    linha->colC = (char*)malloc(sizeof(char*));
    *(linha->colC) = '\0';
  }
}

void drawButtons(int h, EstadoLinha* a, EstadoLinha* b, EstadoLinha* c) {
  drawThreeButtons((h / 3), a);
  drawThreeButtons((h / 3), b);
  drawThreeButtons((h / 3), c);
}
int count = 0;
void drawThreeButtons(int h, EstadoLinha* linha) {
  ImGui::Columns(3, NULL, true);
  if (count > 100000) {
    std::printf("DEBUG texto 154: %p\n", linha->colA);
  }
  criaButton(h, &(linha->colA));
  if (count > 100000) {
    std::printf("DEBUG texto 156: %p\n", linha->colA);
    count = 0;
  }
  count++;
  criaButton(h, &(linha->colB));
  criaButton(h, &(linha->colC));
}

void criaButton(int h, char** texto) {
  ImGui::Spacing();
  ImGui::Button(*texto, ImVec2(ImGui::GetColumnWidth(-1), h));
  if (ImGui::IsItemClicked() && !ENDGAME) {
    std::printf("DEBUG texto 161: %p %p %p %p\n", *texto, player, &PLAYER_X,
                &PLAYER_0);
    *texto = player;
    std::printf("DEBUG texto 164: %p %p %p %p\n", *texto, player, &PLAYER_X,
                &PLAYER_0);
    fmt::print("DEBUG winner: {}\n", (pickWinner()));
    fmt::print("DEBUG endgame: {}\n\n", (isEndgame()));
    trocaPlayer();
  }
  // ImGui::Button(texto, ImVec2(ImGui::GetColumnWidth(-1), h));
  ImGui::Spacing();
  ImGui::NextColumn();
}

void restartButton(int h) {
  ImGui::Columns(1, NULL, true);
  ImGui::Separator();
  // Nx50 button, where N is the remaining width available
  ImGui::Button("Restart Game", ImVec2(-1, h));
  // See also IsItemHovered, IsItemActive, etc
  if (ImGui::IsItemClicked()) {
    fmt::print("Restart pressed.\n");
    limpaLinha(linha1);
    limpaLinha(linha2);
    limpaLinha(linha3);
    player = &PLAYER_X;
    ENDGAME = false;
  }
}

void limpaLinha(EstadoLinha* linha) {
  linha->colA = NULL;
  linha->colB = NULL;
  linha->colC = NULL;
  free(linha->colA);
  free(linha->colB);
  free(linha->colC);
  iniciaLinha(linha);
}

void trocaPlayer() {
  std::printf("DEBUG trocaPlayer 202: %p %c %p %p %p\n", player, *player,
              &PLAYER_X, &PLAYER_0, &(*player));
  player = *player == PLAYER_X ? &PLAYER_0 : &PLAYER_X;
  std::printf("DEBUG trocaPlayer 205: %p %c %p %p %p\n", player, *player,
              &PLAYER_X, &PLAYER_0, &(*player));
}