
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <fstream>

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font* font ;
SDL_Window *savingWindow;

const int SCREEN_S_WIDTH = 400;
const int SCREEN_S_HEIGHT = 300;

using namespace std;
SDL_Color blue = {0 , 139 , 139 , 255};
SDL_Color red = {128 , 0 , 0 , 255};
SDL_Color green = {0 , 255 , 0 , 255};
SDL_Color dark_blue = {0 , 51 , 102 , 255};
SDL_Color dark_red = {139 , 0 , 0 , 255};
SDL_Color dark_green = {0 , 100 , 0 , 255};
SDL_Color dark_yellow = {255 , 140 , 0 , 255};
SDL_Color purple = {128 , 0 , 128 , 255};
SDL_Color gray = {128 , 128 , 128 , 255};
SDL_Color blue_green = {0 , 128 , 128 , 255};
SDL_Color gold = {184 , 134 , 11 , 255};
bool light = true;
vector<vector<string>> lines_copy;
vector<string> lines = {""};
int currentLine = 0;
int cursorPos = 0;

// Global variables for scrolling
int scrollOffset = 0;   // Vertical scrolling offset
int scrollOffsetX = 0;  // Horizontal scrolling offset

struct Keyword {
    regex Pattern;
    SDL_Color color;
    SDL_Color color2;
};
void ensureLastLineVisible(int currentLine, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines, int SCREEN_WIDTH) {
    int cursorY = currentLine * LINE_HEIGHT - scrollOffset + 100; // Start 100px from the top

    if (cursorY < 100) {
        scrollOffset = currentLine * LINE_HEIGHT - 100;
    } else if (cursorY + LINE_HEIGHT > SCREEN_HEIGHT - 10) { // Ensure 10px bottom padding
        scrollOffset = (currentLine + 1) * LINE_HEIGHT - SCREEN_HEIGHT + 10;
    }

    int contentHeight = totalLines * LINE_HEIGHT;
    scrollOffset = max(0, min(scrollOffset, max(0, contentHeight - SCREEN_HEIGHT + 10)));

    // Horizontal scrolling
    if (currentLine >= 0 && currentLine < totalLines) {
        int cursorX = 70; // Start 70px from the left
        TTF_SizeText(TTF_OpenFont("C:\\Windows\\Fonts\\ARLRDBD.TTF", 24), lines[currentLine].substr(0, cursorPos).c_str(), &cursorX, nullptr);

        if (cursorX < scrollOffsetX + 70) {
            scrollOffsetX = cursorX - 70; // Keep 70px left margin
        } else if (cursorX > SCREEN_WIDTH - 10 + scrollOffsetX) {
            scrollOffsetX = cursorX - SCREEN_WIDTH + 10; // Keep 10px right margin
        }
    }
}

void color_changer(SDL_Renderer* r , string a , int y , TTF_Font* font){
    Keyword keywords[12];
    keywords[1].Pattern = (R"(\b(if|while|switch|class)\b)");keywords[1].color = dark_blue;
    // keywords[2].Pattern = (R"(\b(\s*(void|int|float|double)\s+[a-zA-Z_][a-zA-Z0-9_]*\s*)\b)");keywords[2].color = dark_yellow;
    keywords[2].Pattern =(R"(\b(int|float|double)\s+(\w+)\s*(=\s*\w+)?\s*;)");keywords[2].color = dark_red;
    keywords[3].Pattern = (R"(\b(int|float|double)\b)");keywords[3].color = blue_green;
    keywords[4].Pattern =(R"(/"([^"\\]*(\\.[^"\\]*)*)\b"|'([^'\\]*(\\.[^'\\]*)*)')");keywords[4].color=dark_green;
    //  keywords[4].Pattern = (R"(\b(\s*(void|int|float|double)\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\(.*\)\s*)\b)");keywords[4].color = dark_red;
    keywords[5].Pattern = (R"(#\s*include\s*<([^>]+)>)");keywords[5].color = blue;
    keywords[6].Pattern = (R"(\d+)");keywords[6].color = purple;
    keywords[7].Pattern = (R"(\/\/.*|\/\*[^*]*\*+([^/*][^*]*\*+)*\/)");keywords[7].color = gray;
    //keywords[8].Pattern = (R"((\w+|\d+)([\+\-\*\/]+)(\w+|\d+))");keywords[8].color = red;
    keywords[9].Pattern = (R"(([({])[^(){}]*([)}]))");keywords[9].color = gold;

    for (int o = 0; o < 10; ++o) {
        auto words_begin = sregex_iterator(a.begin(), a.end(), keywords[o].Pattern);
        auto words_end = sregex_iterator();

        int xOffset = 10; // X offset for rendering the text
        for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
            smatch match = *it;

            // Get the position of the keyword
            string keyword = match.str();

            // Calculate the pixel offset of the keyword
            int keywordXOffset = 0;
            TTF_SizeText(font, a.substr(0, match.position()).c_str(), &keywordXOffset, nullptr);
            //if(o==8)TTF_SizeText(font, a.substr(0, match.position()).c_str(), &keywordXOffset, nullptr);

            // Render the keyword in highlight color
            SDL_Surface *keywordSurface;
            // if(o==8){keywordSurface =TTF_RenderText_Blended(font, match.str(2).c_str(), keywords[o].color); }
            // else if(o==2){keywordSurface=TTF_RenderText_Blended(font, match.str(2).c_str(), keywords[o].color);}
            // else if(o==9){keywordSurface=TTF_RenderText_Blended(font, match.str(1).c_str(), keywords[o].color);}
            keywordSurface = TTF_RenderText_Blended(font, keyword.c_str(), keywords[o].color);
            if (keywordSurface) {
                SDL_Texture *keywordTexture = SDL_CreateTextureFromSurface(r, keywordSurface);
                SDL_Rect renderQuad = {140 + keywordXOffset, y, keywordSurface->w, keywordSurface->h};
                SDL_RenderCopy(r, keywordTexture, nullptr, &renderQuad);
                SDL_FreeSurface(keywordSurface);
                SDL_DestroyTexture(keywordTexture);
            }
        }
    }
}

// sdl and font functions
bool init_sdl_font() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 0;
    }

    if (TTF_Init() == -1) {
        cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        SDL_Quit();
        return 0;
    }


    window = SDL_CreateWindow("SDL Text Input with New Lines",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              0, 0,
                              SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        cerr << "Failed to create window: " << SDL_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        cerr << "Failed to create renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
    font = TTF_OpenFont("C:\\Windows\\Fonts\\ARLRDBD.TTF", 24);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
    return true;
}
void cleanup() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void saving_file(string text) {
    std::ofstream outputFile("output.cpp");

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
    }

    outputFile << text;

    outputFile.close();
}
string vector_to_string(vector<string> v,int current_line) {
    string text="";
    for (int i=0;i<v.size();i++) {
        text += v[i];
        if (i != v.size()-1) {
            text += "\n";
        }
    }
    return text;
}
void copying_text_to_clipboard(string text) {
    const char * copying=text.c_str();
    if (SDL_SetClipboardText(copying) != 0) {
        std::cerr << "Failed to copy text to clipboard! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Text copied to clipboard: " << copying << std::endl;
    }
}
void pasting_from_clipboard() {
    char* clipboardText = SDL_GetClipboardText();
    if (clipboardText) {
        lines[currentLine].insert(cursorPos, clipboardText);
        SDL_free(clipboardText); // Free the allocated memory
    }
}

int main(int argc, char* argv[]) {

    if (!init_sdl_font()) {return -1;}

    int SCREEN_WIDTH, SCREEN_HEIGHT;
    SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);



    const int LINE_HEIGHT = TTF_FontHeight(font);

    SDL_StartTextInput();

    bool cursorVisible = true;
    Uint32 cursorTimer = SDL_GetTicks();
    const Uint32 CURSOR_BLINK_INTERVAL = 500;

    SDL_Event event;
    bool running = true;

    //second faz
    string selected_text = "";
    int selected_line ;
    int selected_cursorX = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_TEXTINPUT) {
                if (currentLine >= 0 && currentLine < (int)lines.size()) {
                    lines[currentLine].insert(cursorPos, event.text.text);
                    cursorPos++;
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (currentLine >= 0 && currentLine < (int)lines.size()) {
                        if (cursorPos > 0) {
                            lines[currentLine].erase(cursorPos - 1, 1);
                            cursorPos--;
                        } else if (currentLine > 0) {
                            cursorPos = lines[currentLine - 1].size();
                            lines[currentLine - 1] += lines[currentLine];
                            lines.erase(lines.begin() + currentLine);
                            currentLine--;
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2) {
                    if (currentLine >= 0 && currentLine < (int)lines.size()) {
                        string remainder = lines[currentLine].substr(cursorPos);
                        lines[currentLine] = lines[currentLine].substr(0, cursorPos);
                        lines.insert(lines.begin() + currentLine + 1, remainder);
                        currentLine++;
                        cursorPos = 0;
                    }
                } else if (event.key.keysym.sym == SDLK_UP) {
                    if (currentLine > 0) {
                        currentLine--;
                        cursorPos = min(cursorPos, (int)lines[currentLine].size());
                    }
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    if (currentLine < (int)lines.size() - 1) {
                        currentLine++;
                        cursorPos = min(cursorPos, (int)lines[currentLine].size());
                    }
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (cursorPos > 0) {
                        cursorPos--;
                    } else if (currentLine > 0) {
                        currentLine--;
                        cursorPos = lines[currentLine].size();
                    }
                } else if (event.key.keysym.sym == SDLK_RIGHT) {

                    if (cursorPos < (int)lines[currentLine].size()) {
                        cursorPos++;
                    } else if (currentLine < (int)lines.size() - 1) {
                        currentLine++;
                        cursorPos = 0;
                    }
                }//saving system
                else if (event.key.keysym.sym == SDLK_s) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        saving_file(vector_to_string(lines, currentLine));
                    }
                }
                else if (event.key.keysym.sym == SDLK_a) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        string text = vector_to_string(lines, currentLine);
                    }
                }
                else if (event.key.keysym.sym == SDLK_c) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        if(selected_text==""){selected_text=lines[currentLine];}
                        copying_text_to_clipboard(selected_text);
                        selected_text="";
                    }
                }
                else if (event.key.keysym.sym == SDLK_v) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        pasting_from_clipboard();
                    }
                }
                else if (event.key.keysym.sym == SDLK_x) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        if(selected_text=="") {
                            selected_text=lines[currentLine];
                            lines[currentLine]="";
                        }
                        copying_text_to_clipboard(selected_text);

                        selected_text="";
                    }
                }
            }
        }

        if (SDL_GetTicks() - cursorTimer >= CURSOR_BLINK_INTERVAL) {
            cursorVisible = !cursorVisible;
            cursorTimer = SDL_GetTicks();
        }

        ensureLastLineVisible(currentLine, SCREEN_HEIGHT, LINE_HEIGHT, lines.size(), SCREEN_WIDTH);
        if(!light){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
        } else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
        }


        SDL_Color white = {255, 255, 255, 255};
        SDL_Color black = {0, 0, 0, 255};
        //int y = -scrollOffset;
        int xStart = 70;  // Ensure text starts at 70px
        int yStart = 10;  // 10px top margin

        int y = yStart - scrollOffset;

        for (size_t i = 0; i < lines.size(); ++i) {
            if (y + LINE_HEIGHT > yStart && y < SCREEN_HEIGHT - 10) { // Ensure it doesn't exceed bottom limit
                SDL_Color jaddid = light ? SDL_Color{0, 0, 0, 255} : SDL_Color{255, 255, 255, 255};

                SDL_Surface* textSurface = TTF_RenderText_Blended(font, lines[i].c_str(), jaddid);
                if (!textSurface) continue;
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (!textTexture) {
                    SDL_FreeSurface(textSurface);
                    continue;
                }

                // Ensure text starts at 70 pixels and doesn't exceed right margin
                int textWidth = textSurface->w;
                int xPos = max(100, xStart - scrollOffsetX);  // Prevent moving left past 100px
                int maxWidth = SCREEN_WIDTH - 10 - xPos;  // Ensure it doesn't exceed right limit

                SDL_Rect renderQuad = {xPos, y, min(textWidth, maxWidth), textSurface->h};
                SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);

                // Apply syntax highlighting
                color_changer(renderer, lines[i], y, font);
            }

            if (i == currentLine && cursorVisible) {
                int cursorX = 0;
                TTF_SizeText(font, lines[i].substr(0, cursorPos).c_str(), &cursorX, nullptr);
                cursorX = max(100, xStart + cursorX - scrollOffsetX);  // Prevent cursor from moving left past 100px
                SDL_SetRenderDrawColor(renderer, light ? 0 : 255, light ? 0 : 255, light ? 0 : 255, 255);
                SDL_RenderDrawLine(renderer, cursorX, y, cursorX, y + LINE_HEIGHT);
            }

            y += LINE_HEIGHT;
        }


        SDL_RenderPresent(renderer);
    }


    cleanup();
    return 0;
}