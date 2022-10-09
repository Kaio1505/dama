#include "window.hpp"

#include <iostream>

void Window::onCreate() {
  // Load font with bigger size for the X's and O's
  auto
  const filename {
    abcg::Application::getAssetsPath() +
      "arial.ttf"
  };
  auto
  const imagem {
    abcg::Application::getAssetsPath() +
      "gold.png"
  };
  m_font = ImGui::GetIO().Fonts -> AddFontFromFileTTF(filename.c_str(), 72.0f);
  killed_button = 0;
  if (m_font == nullptr) {
    throw abcg::RuntimeError {
      "Cannot load font file"
    };
  }

  restartSelectedField();
  restartGame();
}

void Window::onPaintUI() {
  // Get size of application's window
  auto
  const appWindowWidth {
    gsl::narrow < float > (getWindowSettings().width)
  };
  auto
  const appWindowHeight {
    gsl::narrow < float > (getWindowSettings().height)
  }; {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto
    const flags {
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    };
    ImGui::Begin("Dama", nullptr, flags);

    // Menu
    {
      bool restartSelected {};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
          ImGui::MenuItem("Restart", nullptr, & restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
        restartGame();
      }
    }

    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        text = fmt::format("{}'s turn", m_XsTurn ? "X" : "O");
        break;
      case GameState::Draw:
        text = "Play!";
        break;
      case GameState::WinX:
        text = "X's player wins!";
        break;
      case GameState::WinO:
        text = "O's player wins!";
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
        (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();
    //Set table
    /*{
    	auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
      	auto const buttonHeight{gridHeight / m_N};
      	
      	ImGui::PushFont(m_font);
      	if (ImGui::BeginTable("Game board", m_N)) {
      	
      	}
    }*/
    // Game board
    {
      auto
      const gridHeight {
        appWindowHeight - 22 - 60 - (m_N * 10) - 60
      };
      auto
      const buttonHeight {
        gridHeight / m_N
      };
      auto
      const buttonWidth {
        gridHeight / m_N
      };

      // Use custom font
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i: iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j: iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto
            const offset {
              i * m_N + j
            };

            // Get current character
            auto ch {
              m_board.at(offset)
            };

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (ch == 0) {
              ch = ' ';
            }

            // Button text is ch followed by an ID in the format ##ij
            //auto buttonText{fmt::format("{}##{}{}", ch, i, j)};
            switch (board_color.at(offset)) {
            case 'W':
              actual_color = (ImVec4) ImColor::HSV(0.0f, 0.0f, 100.0f);
              break;
            case 'B':
              actual_color = (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f);
              break;
            case 'G':
              actual_color = (ImVec4) ImColor::HSV(0.33f, 0.5f, 1.0f);
              break;
            default:
              actual_color = (ImVec4) ImColor::HSV(0.33f, 0.5f, 1.0f);
              break;
            }
            if ((i + j) % 2 == 0) {

              ImGui::PushID(offset);
              ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
              ImGui::PushStyleColor(ImGuiCol_Button, actual_color);
              ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.33f, 0.5f, 1.0f));
              ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.33f, 0.5f, 1.0f));
              ImGui::Button(" ", ImVec2(-1, buttonHeight));
              ImGui::PopStyleColor(3);
              ImGui::PopID();
            } else {
              ImGui::PushID(offset);

              ImGui::PushStyleColor(ImGuiCol_Button, actual_color);
              ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.33f, 0.5f, 1.0f));
              ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.33f, 0.5f, 1.0f));

              if (m_board.at(offset) == 'X') {

                if (ImGui::Button("X", ImVec2(-1, buttonHeight))) {

                  restartSelectedField();
                  if (ch == 'X' && m_XsTurn) {
                    activated_button = offset;
                    /*if((i-1) * (m_N) + j-1>0  && m_board.at((i-1) * (m_N) + j-1)!='X' && i!=0 && (j)%8!=0){
                    	
                    	board_color.at((i-1) * (m_N) + j-1) = 'G';
                    }
                    if((i-1) * (m_N) + j+1>0 && m_board.at((i-1) * (m_N) + j+1)!='X' && i!=0 && (j+1)%8!=0){
                    	board_color.at((i-1) * (m_N) + j+1) = 'G';
                    }*/
                    if ((i + 1) * (m_N) + j - 1 < (m_N * m_N) && m_board.at((i + 1) * (m_N) + j - 1) != 'X' && i != m_N - 1 && (j) % 8 != 0) {

                      if (i + 2 <= m_N - 1 && j - 2 >= 0 && m_board.at((i + 1) * (m_N) + j - 1) == 'O' && m_board.at((i + 2) * (m_N) + j - 2) != 'X' && m_board.at((i + 2) * (m_N) + j - 2) != 'O' && m_XsTurn) {
                        board_color.at((i + 2) * (m_N) + j - 2) = 'G';
                        killed_button = (i + 1) * (m_N) + j - 1;
                      } else if (m_board.at((i + 1) * (m_N) + j - 1) != 'O') {
                        board_color.at((i + 1) * (m_N) + j - 1) = 'G';
                      }
                    }
                    if ((i + 1) * (m_N) + j + 1 < (m_N * m_N) && m_board.at((i + 1) * (m_N) + j + 1) != 'X' && i != m_N - 1 && (j + 1) % 8 != 0) {

                      if (i + 2 <= m_N - 1 && j + 2 <= m_N - 1 && m_board.at((i + 1) * (m_N) + j + 1) == 'O' && m_board.at((i + 2) * (m_N) + j + 2) != 'X' && m_board.at((i + 2) * (m_N) + j + 2) != 'O' && m_XsTurn) {
                        board_color.at((i + 2) * (m_N) + j + 2) = 'G';
                        killed_button = (i + 1) * (m_N) + j + 1;
                      } else if (m_board.at((i + 1) * (m_N) + j + 1) != 'O') {
                        board_color.at((i + 1) * (m_N) + j + 1) = 'G';
                      }
                    }
                    board_color.at(offset) = 'G';
                  }
                }
              }
              if (m_board.at(offset) == 'O') {
                if (ImGui::Button("O", ImVec2(-1, buttonHeight))) {
                  restartSelectedField();
                  if (ch == 'O' && !m_XsTurn) {
                    activated_button = offset;
                    if ((i - 1) * (m_N) + j - 1 > 0 && m_board.at((i - 1) * (m_N) + j - 1) != 'O' && i != 0 && (j) % 8 != 0) {

                      if (i - 2 >= 0 && j - 2 >= 0 && m_board.at((i - 1) * (m_N) + j - 1) == 'X' && m_board.at((i - 2) * (m_N) + j - 2) != 'X' && m_board.at((i - 2) * (m_N) + j - 2) != 'O' && !m_XsTurn) {
                        board_color.at((i - 2) * (m_N) + j - 2) = 'G';
                        killed_button = (i - 1) * (m_N) + j - 1;
                      } else if (m_board.at((i - 1) * (m_N) + j - 1) != 'X') {
                        board_color.at((i - 1) * (m_N) + j - 1) = 'G';
                      }
                    }
                    if ((i - 1) * (m_N) + j + 1 > 0 && m_board.at((i - 1) * (m_N) + j + 1) != 'O' && i != 0 && (j + 1) % 8 != 0) {

                      if (i - 2 >= 0 && j + 2 <= m_N - 1 && m_board.at((i - 1) * (m_N) + j + 1) == 'X' && m_board.at((i - 2) * (m_N) + j + 2) != 'X' && m_board.at((i - 2) * (m_N) + j + 2) != 'O' && !m_XsTurn) {
                        board_color.at((i - 2) * (m_N) + j + 2) = 'G';
                        killed_button = (i - 1) * (m_N) + j + 1;
                      } else if (m_board.at((i - 1) * (m_N) + j + 1) != 'X') {
                        board_color.at((i - 1) * (m_N) + j + 1) = 'G';
                      }
                    }
                    /*if((i+1)* (m_N) + j-1<(m_N*m_N) && m_board.at((i+1)* (m_N) + j-1)!='O' && i!=m_N-1 && (j)%8!=0){
                    	board_color.at((i+1) * (m_N) + j-1) = 'G';
                    }
                    if((i+1)* (m_N) + j+1<(m_N*m_N) && m_board.at((i+1)* (m_N) + j+1)!='O' && i!=m_N-1 && (j+1)%8!=0){
                    	board_color.at((i+1) * (m_N) + j+1) = 'G';
                    }*/
                    board_color.at(offset) = 'G';
                  }
                }
              }
              if (m_board.at(offset) != 'O' && m_board.at(offset) != 'X') {
                if (ImGui::Button(" ", ImVec2(-1, buttonHeight)) && board_color.at(offset) == 'G') {
                  m_board.at(activated_button) = ' ';
                  if (m_XsTurn) {
                    m_board.at(offset) = 'X';
                  } else {
                    m_board.at(offset) = 'O';
                  }
                  m_board.at(killed_button) = ' ';
                  killed_button = 0;
                  restartSelectedField();
                  m_XsTurn = !m_XsTurn;
                  checkEndCondition();
                }
              }
              ImGui::PopStyleColor(3);
              ImGui::PopID();
            }

            /*if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_gameState == GameState::Play && ch == ' ') {
                m_board.at(offset) = m_XsTurn ? 'X' : 'O';
                //checkEndCondition();
                m_XsTurn = !m_XsTurn;
              }
            }*/
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::Spacing();
    // "Restart game" button
    {
      if (ImGui::Button("Restart game", ImVec2(-1, 50))) {
        restartGame();
      }
    }

    ImGui::End();
  }
}

void Window::restartSelectedField() {
  for (auto i: iter::range(m_N)) {
    for (auto j: iter::range(m_N)) {
      if ((i + j) % 2 == 0) {
        board_color.at(i * m_N + j) = 'W';
      } else {
        board_color.at(i * m_N + j) = 'B';

      }
    }
  }
}

void Window::checkEndCondition() {
  count_X = 0;
  count_Y = 0;
  for (auto i: iter::range(m_N)) {
    for (auto j: iter::range(m_N)) {
      if (m_board.at(i * m_N + j) == 'X') {
        count_X++;
      }
      if (m_board.at(i * m_N + j) == 'O') {
        count_Y++;
      }
    }
  }
  if (count_X == 0) {
    m_gameState = GameState::WinO;
  }
  if (count_Y == 0) {
    m_gameState = GameState::WinX;
  }
}

void Window::restartGame() {
  m_board.fill('\0');
  m_gameState = GameState::Play;
  m_XsTurn = true;
  for (auto i: iter::range(m_N)) {
    for (auto j: iter::range(m_N)) {
      if ((i + j) % 2 == 0) {
        board_color.at(i * m_N + j) = 'W';
      } else {
        board_color.at(i * m_N + j) = 'B';
        if (i < 3) {
          m_board.at(i * m_N + j) = 'X';
        }
        if (i >= 5) {
          m_board.at(i * m_N + j) = 'O';
        }
      }
    }
  }

}
