
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <iostream>
    #include <string>
    #include <vector>
    #include <algorithm>
    #include <regex>
    #include <fstream>
    using namespace std;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font* font ;
    SDL_Window *savingWindow;
    const int OFFSET_X = 200;
    const int OFFSET_Y = 50;
    const int MAX_VERTICAL_POSITION = 1000;
    bool first_comment = false;
    bool second_comment = false;
    int last_line = -1;
    char last_char = ' ';
    const int SCREEN_S_WIDTH = 400;
    const int SCREEN_S_HEIGHT = 300;
    SDL_Rect rect1 = {0, 0, 1950, 50};
    SDL_Rect rect2 = {0, 0, 200, 1200};
    SDL_Rect rect3 = {200, 0, 350, 50};
    SDL_Rect rect4 = {550, 0, 350, 50};
    SDL_Rect rect5 = {900, 0, 350, 50};
    SDL_Rect rect6 = {1250, 0, 350, 50};
    SDL_Rect rect7 = {1600, 0, 350, 50};
    SDL_Rect rect8 = {300, 50, 150, 25};
    SDL_Rect rect9 = {300, 75, 150, 25};
    SDL_Rect rect10 = {300, 100, 150, 25};
    SDL_Rect rect11 = {650, 50, 150, 25};
    SDL_Rect rect12 = {650, 75, 150, 25};
    SDL_Rect rect13 = {1000, 75, 150, 25};
    SDL_Rect rect14 = {1000, 50, 150, 25};
    SDL_Rect error = {0, 880, 1950, 200};
    // Define the rectangle (x, y, w, h) for text input area
    SDL_Rect rec = {200, 50, 1750, 950};  // x = 200, y = 50, w = 1750, h = 950

    SDL_Color ercolor = {0 , 255 , 255 , 255};
    SDL_Color white = {255 , 255 , 255 , 255};
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
    SDL_Color black = {0, 0, 0, 255};
    bool light = true;
    vector<vector<string>> lines_copy;
    vector<string> lines = {""};
    int currentLine = 0;
    int cursorPos = 0;
    int error_lines;
    // Global variables for scrolling
    int scrollOffset = 0;   // Vertical scrolling offset
    int scrollOffsetX = 0;  // Horizontal scrolling offset
    bool file_button_clicked = false;
    bool edit_button_clicked = false;
    bool run_button_clicked = false;
    bool view_button_clicked = false;
    bool debug_button_clicked = false;
    bool iostream_library = false;
    bool cmath_library = false;
    bool bitsstdch = false;
    bool usingnamespace = false;
    bool library_error = false;
    struct Keyword {
        regex Pattern;
        SDL_Color color;
        SDL_Color color2;
    };
    bool mouse_place_checker(int x, int y, SDL_Rect rect) {
        return x >= rect.x && x <= rect.x + rect.w &&
               y >= rect.y && y <= rect.y + rect.h;
    }

    void ensureLastLineVisible(int currentLine, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines, int SCREEN_WIDTH) {
        int cursorY = currentLine * LINE_HEIGHT - scrollOffset + OFFSET_Y;

        if (cursorY < OFFSET_Y) {
            scrollOffset = currentLine * LINE_HEIGHT;
        } else if (cursorY + LINE_HEIGHT > SCREEN_HEIGHT + OFFSET_Y) {
            scrollOffset = (currentLine + 1) * LINE_HEIGHT - SCREEN_HEIGHT;
        }

        int contentHeight = totalLines * LINE_HEIGHT;
        scrollOffset = max(0, min(scrollOffset, max(0, contentHeight - SCREEN_HEIGHT)));

        // Horizontal scrolling
        if (currentLine >= 0 && currentLine < totalLines) {
            int cursorX = 0;
            // Get width of the current line up to cursor position
            TTF_SizeText(TTF_OpenFont("C:\\Windows\\Fonts\\ARLRDBD.TTF", 24), lines[currentLine].substr(0, cursorPos).c_str(), &cursorX, nullptr);
            cursorX+=OFFSET_X;
            if (cursorX < scrollOffsetX) {
                scrollOffsetX = cursorX; // Move left
            } else if (cursorX > SCREEN_WIDTH + scrollOffsetX) {
                scrollOffsetX = cursorX - SCREEN_WIDTH; // Move right
            }
        }
    }void renderText(const std::string& text, SDL_Rect rect, SDL_Color color , TTF_Font* font) {
        // Create text surface
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (!textSurface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            return;
        }

        // Create texture from surface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        if (!textTexture) {
            std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
            return;
        }

        // Render text inside the rect
        SDL_RenderCopy(renderer, textTexture, NULL, &rect);

        // Clean up
        SDL_DestroyTexture(textTexture);
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

            int xOffset = 0; // X offset for rendering the text
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
                    SDL_Rect renderQuad = { keywordXOffset, y, keywordSurface->w, keywordSurface->h};
                    SDL_RenderCopy(r, keywordTexture, nullptr, &renderQuad);
                    SDL_FreeSurface(keywordSurface);
                    SDL_DestroyTexture(keywordTexture);
                }
            }
        }
    }
    void library_handler() {
        regex iostream1(R"(#include\s*[\"<]iostream[\">])"); // Corrected raw string syntax
        regex cmathPattern(R"(#include\s*[\"<]cmath[\">])");         // Matches #include <cmath> or #include "cmath"
        regex bitsStdHPattern(R"(#include\s*[\"<]bits/stdc\+\+\.h[\">])"); // Matches #include <bits/stdc++.h> or #include "bits/stdc++.h"
        regex mathFunctionsPattern(R"(std::(sqrt|pow|sin|cos|tan|abs|exp|log|log10|floor|ceil)\s*\([^\)]*\))");
        regex ioFunctionsPattern(R"(std::(cout|cin|endl|getline))");

        for (int i = 0; i < lines.size(); ++i) {
            if (regex_search(lines[i], iostream1)) {
                iostream_library = true;  // Set to true when the first library is found
            } if (regex_search(lines[i], cmathPattern)) {
                cmath_library = true;  // Set to true when the second library is found after the first
            }if (regex_search(lines[i], bitsStdHPattern)) {
                bitsstdch = true;
                cmath_library = true;
                iostream_library = true;
                // Set to true when the third library is found after the first two
            }
            if(!iostream_library && regex_search(lines[i], ioFunctionsPattern)){
                SDL_Rect error2 = {0 ,880 , 1950 , 25 * (error_lines + 1)};
                error_lines++;
                renderText("error : iostream library doesnt exist .                                                                                                                                                          ", error2, ercolor , font);
                return;
            }
            if(!cmath_library && regex_search(lines[i], mathFunctionsPattern)){
                SDL_Rect error2 = {0 ,880 , 1950 , 25 * (error_lines + 1)};
                error_lines++;
                renderText("error : cmath library doesnt exist .                                                                                                                                                             ", error2, ercolor , font);
                return;
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
    void file_button() {
        if (light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect8);
        SDL_RenderFillRect(renderer, &rect9);
        SDL_RenderFillRect(renderer, &rect10);
        if (!light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect8);
        SDL_RenderDrawRect(renderer, &rect9);
        SDL_RenderDrawRect(renderer, &rect10);
        renderText("     New Project    ", rect8, green, font);
        renderText("     Save Project    ", rect9, green, font);
        renderText("         Exit        ", rect10, green, font);
    }
    void view_button() {
        if (light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect11);
        SDL_RenderFillRect(renderer, &rect12);
        if (!light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect11);
        SDL_RenderDrawRect(renderer, &rect12);
        renderText("     Dark Mode     ", rect11, green, font);
        renderText("     Light Mode    ", rect12, green, font);
    }
    void edit_button() {
        if (light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect13);
        SDL_RenderFillRect(renderer, &rect14);
        if (!light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect13);
        SDL_RenderDrawRect(renderer, &rect14);
        renderText("         Undo        ", rect13, green, font);
        renderText("         Redo        ", rect14, green, font);
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
        int mouseX, mouseY;
        Uint32 mousestate=SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if(event.type==SDL_MOUSEBUTTONDOWN) {

                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (mouse_place_checker(mouseX, mouseY, rect11) && view_button_clicked){
                        light = false;
                        //cout<<"kkkkk";
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect12) && view_button_clicked){
                        light = true;
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect6)){
                        //compiling
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect7)){
                        //run
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect13) && edit_button_clicked){
                        //Undo
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect14) && edit_button_clicked){
                        //redo
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect8) && file_button_clicked){
                        //new
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect9) && file_button_clicked){
                        //save
                    }
                    if (mouse_place_checker(mouseX, mouseY, rect10) && file_button_clicked){
                        //exit
                        running = false;
                    }
                    //cout<<"kkkkk";
                    if (mouse_place_checker(mouseX, mouseY, rect3)) {
                        file_button_clicked = true;
                        //cout<<"kkkkk";
                    } else file_button_clicked = false;
                    if (mouse_place_checker(mouseX, mouseY, rect5)) {
                        edit_button_clicked = true;
                        //cout<<"kkkkk";
                    } else edit_button_clicked = false;
                    if (mouse_place_checker(mouseX, mouseY, rect4)) {
                        view_button_clicked = true;
                        //cout<<"kkkkk";
                    } else view_button_clicked = false;

                }
            }
            else if (event.type == SDL_TEXTINPUT) {
                if (currentLine >= 0 && currentLine < (int)lines.size()) {
                    lines[currentLine].insert(cursorPos, event.text.text);
                    cursorPos++;
 //                   if(event.text.text == "/" && last_char == '/' && last_line == currentLine)first_comment= true;
//                    if(last_line != currentLine ) first_comment = false;
//                    if(event.text.text == "*" && last_char == '/' && last_line == currentLine)second_comment = true;
//                   if(event.text.text == "/" && last_char == '*' && last_line == currentLine)second_comment= false;
                }
            }else if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scrollOffset = max(scrollOffset - LINE_HEIGHT, 0);  // Scroll up
                } else if (event.wheel.y < 0) {
                    int totalTextHeight = lines.size() * LINE_HEIGHT;
                    int maxScrollOffset = totalTextHeight - SCREEN_HEIGHT;
                    scrollOffset = min(scrollOffset + LINE_HEIGHT, maxScrollOffset);
                }
            }
            else if (event.type == SDL_KEYDOWN) {
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
                        scrollOffset = max(scrollOffset - LINE_HEIGHT, 0);  // Scroll up
                    }
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    if (currentLine < (int)lines.size() - 1) {
                        currentLine++;
                        cursorPos = min(cursorPos, (int)lines[currentLine].size());
                        int totalTextHeight = lines.size() * LINE_HEIGHT;
                        int maxScrollOffset = totalTextHeight - SCREEN_HEIGHT;
                        scrollOffset = min(scrollOffset + LINE_HEIGHT, maxScrollOffset);
                    }
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (cursorPos > 0) {
                        cursorPos--;
                    } else if (currentLine > 0) {
                        currentLine--;
                        cursorPos = lines[currentLine].size();
                    }

                    // Adjust horizontal scrolling if the cursor goes beyond the visible area
                    int textWidth = 0;
                    TTF_SizeText(font, lines[currentLine].c_str(), &textWidth, nullptr);
                    // Scroll left when the cursor goes beyond the visible area
                    scrollOffsetX = max(scrollOffsetX - 10, 0);
                }

                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    if (cursorPos < (int)lines[currentLine].size()) {
                        cursorPos++;
                    } else if (currentLine < (int)lines.size() - 1) {
                        currentLine++;
                        cursorPos = 0;
                    }

                    // Adjust horizontal scrolling if the cursor goes beyond the visible area
                    int textWidth = 0;
                    TTF_SizeText(font, lines[currentLine].c_str(), &textWidth, nullptr);
                    // Scroll right if the cursor moves beyond the screen width
                    scrollOffsetX = min(scrollOffsetX + 10, max(0, textWidth - SCREEN_WIDTH));
                }
//saving system
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

        int y = -scrollOffset + OFFSET_Y;
        int maxTextWidth = 0;
        for (size_t i = 0; i < lines.size(); ++i) {
            int lineWidth = 0;
            TTF_SizeText(font, lines[i].c_str(), &lineWidth, nullptr);
            maxTextWidth = max(maxTextWidth, lineWidth);
            if (y + LINE_HEIGHT > 0 && y < SCREEN_HEIGHT) {
                SDL_Color jaddid = {0, 0, 0, 255};
                if(!light){
                    jaddid = {255, 255, 255, 255};
                }
                SDL_Surface* textSurface = TTF_RenderText_Blended(font, lines[i].c_str(), jaddid);
                if (!textSurface) continue;
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (!textTexture) {
                    SDL_FreeSurface(textSurface);
                    continue;
                }

                SDL_Rect renderQuad = {OFFSET_X, y, textSurface->w, textSurface->h};
                if (y >= MAX_VERTICAL_POSITION) {
                    break; // Stop rendering text if we've exceeded the maximum vertical position
                }
                SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                // Define regex pattern to match keywords "if" and "while"
                color_changer(renderer , lines[i] , y , font);
            }

            if (i == currentLine && cursorVisible) {
                int cursorX = 0;
                TTF_SizeText(font, lines[i].substr(0, cursorPos).c_str(), &cursorX, nullptr);
                cursorX += OFFSET_X;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                if(light){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                }
                SDL_RenderDrawLine(renderer, cursorX - scrollOffsetX, y, cursorX - scrollOffsetX, y + LINE_HEIGHT);
            }

            y += LINE_HEIGHT;
        }
        scrollOffsetX = max(0, min(scrollOffsetX, maxTextWidth - SCREEN_WIDTH));
        if(light)
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        else SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);
        SDL_RenderFillRect(renderer, &rect2);
        if(light)
        SDL_SetRenderDrawColor(renderer, 140, 140, 140, 255);
        else SDL_SetRenderDrawColor(renderer, 93, 93, 93, 255);
        SDL_RenderFillRect(renderer, &rect1);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer , &rect3);
        SDL_RenderDrawRect(renderer , &rect4);
        SDL_RenderDrawRect(renderer , &rect5);
        SDL_RenderDrawRect(renderer , &rect6);
        SDL_RenderDrawRect(renderer , &rect7);
        renderText("           File          ", rect3, black , font);
        renderText("           View          ", rect4, black , font);
        renderText("           Edit          ", rect5, black , font);
        renderText("     Debug & Compile     ", rect6, black , font);
        renderText("           Run           ", rect7, black , font);
        if(light)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer , &error);
        SDL_RenderFillRect(renderer, &error);
        if(file_button_clicked)file_button();
        if(edit_button_clicked)edit_button();
        if(view_button_clicked)view_button();
        library_handler();
        error_lines = 0;
        SDL_RenderPresent(renderer);
    }


    cleanup();
    return 0;
}