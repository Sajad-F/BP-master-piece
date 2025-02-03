
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <fstream>
#include <dirent.h>
using namespace std;
vector<string> clipboard; // Stores copied text
bool selecting = false;
int selectionStartLine = -1, selectionStartPos = -1;
int selectionEndLine = -1, selectionEndPos = -1;
string line_find = "";
bool lining= false;
bool abcd = false;
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font* font ;
SDL_Window *savingWindow;
string name_place = "";
std::vector<std::string> directoryTree;
std::string path = "D:\\Main project BP\\";  // Change to your directory
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
SDL_Rect save_name = {500 , 500 , 900 , 50};

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
SDL_Color phiroze_blue = {0 , 139 , 139 , 255};
//dark mode colors
SDL_Color d_purple ={198,120,221,225};
SDL_Color d_red = {224 , 108 , 117 , 255};
SDL_Color d_light_blue   = {97 , 175 , 254 , 255};
SDL_Color d_yellow = {229 , 192 , 123 , 255};
SDL_Color d_green = {152 , 195 , 121 , 255};
SDL_Color d_orange = {209 , 154 , 102 , 255};
SDL_Color d_gray ={92,99,112,255};
SDL_Color d_phiroze_blue = {86,182,194,255};
SDL_Color d_dark_orange ={213,94,0,255};
SDL_Color d_light_gray = {171,178,191,85};
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
vector<string> variable_names;
std::vector<std::pair<SDL_Rect, std::string>> filePositions;
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}
void get_directory_tree(const std::string& dir_path, std::vector<std::string>& tree) {
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        tree.push_back(entry->d_name);
    }

    closedir(dir);
}
void renderText1(const std::vector<std::string>& cakes) {
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
//        SDL_RenderClear(renderer);

    //       SDL_Color white = {255, 255, 255, 255};
    int rt = 50;  // Starting Y position
    filePositions.clear();
    for (int u = 0 ; u < cakes.size() ; u++) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, cakes[u].c_str(), black);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect renderQuad = {20, rt,min( textSurface->w , 180), textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
        filePositions.push_back({renderQuad, cakes[u]});
        rt += 25;  // Move down for next line

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // SDL_RenderPresent(renderer);
}
bool mouse_place_checker(int x, int y, SDL_Rect rect) {
    return x >= rect.x && x <= rect.x + rect.w &&
           y >= rect.y && y <= rect.y + rect.h;
}

void ensureLastLineVisible(int currentLine, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines, int SCREEN_WIDTH) {
    int cursorY = currentLine * LINE_HEIGHT - scrollOffset ;

    if (cursorY < -OFFSET_Y) {
        scrollOffset = currentLine * LINE_HEIGHT ;
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
void renderRegexGroupsForMod1(SDL_Renderer *r, TTF_Font *font, const std::string &text, int y) {
    // Define a regex pattern with capturing groups
    std::regex pattern(R"(\b(int|float|double)\b\s+(\w+)\s*((\+|\-|\/|\*)?\s*=\s*(\d+|\w+))?\s*;)");
    // Group 1: "int" (keyword)
    // Group 2: variable name
    // Group 3: numeric value

    // Define colors for each group
    SDL_Color colors[] = {
            blue_green,   //  for int or float or double
            dark_red,   //  for variable name
            d_red,
            d_yellow
    };

    // Iterate over matches
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
        std::smatch match = *it;

        //push_back variable name
        variable_names.push_back(match.str(2));

        // Calculate the pixel offset of the entire match
        int matchXOffset = 0;
        TTF_SizeText(font, text.substr(0, match.position()).c_str(), &matchXOffset, nullptr);

        // Render each captured group with its respective color
        for (size_t i = 1; i <= match.size() - 2; ++i) {
            std::string groupText = match.str(i);

            // Calculate the pixel offset of the current group
            int groupXOffset = 0;
            TTF_SizeText(font, text.substr(0, match.position(i)).c_str(), &groupXOffset, nullptr);

            // Render the group text
            SDL_Surface *groupSurface;
            if(light)
                groupSurface = TTF_RenderText_Blended(font, groupText.c_str(), colors[i - 1]);
            else groupSurface = TTF_RenderText_Blended(font, groupText.c_str(), colors[i + 1]);
            if (groupSurface) {
                SDL_Texture *groupTexture = SDL_CreateTextureFromSurface(r, groupSurface);
                SDL_Rect renderQuad = {200 + groupXOffset, y, groupSurface->w, groupSurface->h};
                SDL_RenderCopy(r, groupTexture, nullptr, &renderQuad);
                SDL_FreeSurface(groupSurface);
                SDL_DestroyTexture(groupTexture);
            }
        }
    }
}
void renderRegexGroupsForMod2(SDL_Renderer *r, TTF_Font *font, const std::string &text, int y) {
    // Define a regex pattern with capturing groups
    std::regex pattern(R"(\b(void|int|float|double)\s+(\w+)\(([^)]*)\)\s*)");
    // Group 1: "int" (keyword)
    // Group 2: variable name


    // Define colors for each group
    SDL_Color colors[] = {
            blue_green,   //  for int or float or double
            dark_yellow,   //  for variable name
            d_red,
            d_light_blue
    };

    // Iterate over matches
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
        std::smatch match = *it;

        // Calculate the pixel offset of the entire match
        int matchXOffset = 0;
        TTF_SizeText(font, text.substr(0, match.position()).c_str(), &matchXOffset, nullptr);

        // Render each captured group with its respective color
        for (size_t i = 1; i <= match.size() - 2; ++i) {
            std::string groupText = match.str(i);

            // Calculate the pixel offset of the current group
            int groupXOffset = 0;
            TTF_SizeText(font, text.substr(0, match.position(i)).c_str(), &groupXOffset, nullptr);

            // Render the group text
            SDL_Surface *groupSurface;
            if(light)
                groupSurface = TTF_RenderText_Blended(font, groupText.c_str(), colors[i - 1]);
            else groupSurface = TTF_RenderText_Blended(font, groupText.c_str(), colors[i + 1]);
            if (groupSurface) {
                SDL_Texture *groupTexture = SDL_CreateTextureFromSurface(r, groupSurface);
                SDL_Rect renderQuad = {200 + groupXOffset, y, groupSurface->w, groupSurface->h};
                SDL_RenderCopy(r, groupTexture, nullptr, &renderQuad);
                SDL_FreeSurface(groupSurface);
                SDL_DestroyTexture(groupTexture);
            }
        }
    }
}
void color_changer(SDL_Renderer* r , string a , int y , TTF_Font* font){
    Keyword keywords[12];
    static bool in_comment = false;
    keywords[1].Pattern = (R"(\b(if|while|switch|class)\b)");keywords[1].color = dark_blue;keywords[1].color2=d_purple;

    keywords[4].Pattern =(R"("([^"\\]*(\\.[^"\\]*)*)\b"|'([^'\\]*(\\.[^'\\]*)*)')");keywords[4].color=dark_green;keywords[4].color2=d_green;
    keywords[5].Pattern = (R"(#\s*include\s*<([^>]+)>)");keywords[5].color = phiroze_blue;keywords[5].color2=d_phiroze_blue;
    keywords[6].Pattern = (R"(\d+)");keywords[6].color = purple;keywords[6].color2=d_orange;
    keywords[7].Pattern = (R"(\/\/.*|\/\*[^*]*\*+([^/*][^*]*\*+)*\/)");keywords[7].color = gray;keywords[7].color2=d_gray;
    keywords[8].Pattern = (R"((\+|\-|\*|\/))");keywords[8].color = red;keywords[8].color2=d_dark_orange;
    keywords[9].Pattern = (R"(\(|\)|\{|\}|\[|\])");keywords[9].color = gold;keywords[9].color2=d_light_gray;
    regex iostream1(R"(#include\s*[\"<]iostream[\">])"); // Corrected raw string syntax
    regex cmathPattern(R"(#include\s*[\"<]cmath[\">])");         // Matches #include <cmath> or #include "cmath"
    regex bitsStdHPattern(R"(#include\s*[\"<]bits/stdc\+\+\.h[\">])"); // Matches #include <bits/stdc++.h> or #include "bits/stdc++.h"
    regex mathFunctionsPattern(R"(std::(sqrt|pow|sin|cos|tan|abs|exp|log|log10|floor|ceil)\s*\([^\)]*\))");
    regex ioFunctionsPattern(R"(std::(cout|cin|endl|getline))");
    keywords[2].Pattern=mathFunctionsPattern;keywords[2].color=red;keywords[2].color2=d_red;
    keywords[3].Pattern=ioFunctionsPattern;keywords[3].color=red;keywords[3].color2=d_red;
    renderRegexGroupsForMod1(r,font,a,y);
    renderRegexGroupsForMod2(r,font,a,y);
    for (int o = 0; o < 10; ++o) {
        auto words_begin = sregex_iterator(a.begin(), a.end(), keywords[o].Pattern);
        auto words_end = sregex_iterator();

        int xOffset = 10; // X offset for rendering the text

        for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
            smatch match = *it;

            // Get the position of the keyword
            string keyword = match.str();
            if(o==8){
                if (keyword == "/") {
                    size_t pos = match.position();
                    if (pos + 1 < a.size() && (a[pos + 1] == '/'||a[pos + 1] == '*')) {
                        continue; // Skip this match
                    }
                    if(pos -1 >= 0 && (a[pos - 1] == '/'||a[pos - 1] == '*')) {
                        continue;
                    }
                }
                if (keyword == "*") {
                    size_t pos = match.position();
                    if (pos + 1 < a.size() && (a[pos + 1] == '/')) {
                        continue; // Skip this match
                    }
                    if(pos -1 >= 0 && (a[pos - 1] == '/')) {
                        continue;
                    }
                }
            }
            // Calculate the pixel offset of the keyword
            int keywordXOffset = 0;
            TTF_SizeText(font, a.substr(0, match.position()).c_str(), &keywordXOffset, nullptr);

            // Render the keyword in highlight color
            SDL_Surface *keywordSurface;
//                if (regex_search(a, iostream1)) {
//                    iostream_library = true;  // Set to true when the first library is found
//                } if (regex_search(a, cmathPattern)) {
//                    cmath_library = true;  // Set to true when the second library is found after the first
//                }if (regex_search(a, bitsStdHPattern)) {
//                    bitsstdch = true;
//                    cmath_library = true;
//                    iostream_library = true;
//                    // Set to true when the third library is found after the first two
//                }
            // if(o==8){keywordSurface =TTF_RenderText_Blended(font, match.str(2).c_str(), keywords[o].color); }
            // else if(o==2){keywordSurface=TTF_RenderText_Blended(font, match.str(2).c_str(), keywords[o].color);}
            // else if(o==9){keywordSurface=TTF_RenderText_Blended(font, match.str(1).c_str(), keywords[o].color);}
            if(o==2||o==3){if(o==2&&!cmath_library){keywordSurface=TTF_RenderText_Blended(font, keyword.c_str(), keywords[o].color);}
                if(o==2&&cmath_library){
                    continue;
                    keywordSurface=TTF_RenderText_Blended(font, keyword.c_str(), {255,255,255,255});
                    cout<<"kkkkkk";
                }
                if(o==3&&!iostream_library){keywordSurface=TTF_RenderText_Blended(font, keyword.c_str(), keywords[o].color);}}
            if(o==3&&iostream_library){
                continue;
                keywordSurface=TTF_RenderText_Blended(font, keyword.c_str(), {255,255,255,255});}
            else {
                if(light)
                    keywordSurface = TTF_RenderText_Blended(font, keyword.c_str(), keywords[o].color);
                if(!light)
                    keywordSurface = TTF_RenderText_Blended(font, keyword.c_str(), keywords[o].color2);
            }
            if (!keywordSurface) {
                std::cerr << "Error: Failed to render text: " << keyword << std::endl;
                continue; // Skip this iteration
            }
            if (keywordSurface) {
                SDL_Texture *keywordTexture = SDL_CreateTextureFromSurface(r, keywordSurface);
                SDL_Rect renderQuad = {200 + keywordXOffset, y, keywordSurface->w, keywordSurface->h};
                SDL_RenderCopy(r, keywordTexture, nullptr, &renderQuad);
                SDL_FreeSurface(keywordSurface);
                SDL_DestroyTexture(keywordTexture);
            }
        }
    }

//        for (int i = 0; i < lines.size(); ++i) {
//            if (regex_search(lines[i], iostream1)) {
//                iostream_library = true;  // Set to true when the first library is found
//            } if (regex_search(lines[i], cmathPattern)) {
//                cmath_library = true;  // Set to true when the second library is found after the first
//            }if (regex_search(lines[i], bitsStdHPattern)) {
//                bitsstdch = true;
//                cmath_library = true;
//                iostream_library = true;
//                // Set to true when the third library is found after the first two
//            }
//            if(!iostream_library && regex_search(lines[i], ioFunctionsPattern)){
//                SDL_Rect error2 = {0  ,880 + error_lines*25, 1950 , 25 * (error_lines + 1)};
//                error_lines++;
//                renderText("error : iostream library doesnt exist .                                                                                                                                                       ", error2, ercolor , font);
//                return;
//            }
//            if(!cmath_library && regex_search(lines[i], mathFunctionsPattern)){
//                SDL_Rect error2 = {0 ,880 , 1950 ,     25 * (error_lines + 1)};
//                error_lines++;
//                renderText("error : cmath library doesnt exist .                                                                                                                                                             ", error2, ercolor , font);
//                return;
//            }
//        }
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

void saving_file(const std::string& filename, const std::string& text) {
    std::ofstream outputFile(filename);  // Save using the provided filename

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return;
    }

    outputFile << text;
    outputFile.close();
    std::cout << "File saved successfully as: " << filename << std::endl;
}
bool saving = false;
void save_details(){


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
void select_all_text() {
    selectionStartLine = 0; // Starting from the first line
    selectionEndLine = lines.size() - 1; // Last line of the text
    selectionStartPos = 0; // Start from the beginning of the first line
    selectionEndPos = lines[selectionEndLine].size(); // End at the end of the last line
}
std::vector<std::string> string_to_vector(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    return lines;
}
void copying_text_to_clipboard() {
    clipboard.clear(); // Clear previous clipboard content

    if (selectionStartLine == -1 || selectionEndLine == -1) {
        // No selection, copy the whole current line
        clipboard.push_back(lines[currentLine]);
    } else {
        // Multi-line selection
        int startLine = min(selectionStartLine, selectionEndLine);
        int endLine = max(selectionStartLine, selectionEndLine);
        int startPos = (selectionStartLine < selectionEndLine) ? selectionStartPos : selectionEndPos;
        int endPos = (selectionStartLine < selectionEndLine) ? selectionEndPos : selectionStartPos;

        string copiedText;
        for (int i = startLine; i <= endLine; ++i) {
            int lineStart = (i == startLine) ? startPos : 0;
            int lineEnd = (i == endLine) ? endPos : lines[i].size();

            string selectedPart = lines[i].substr(lineStart, lineEnd - lineStart);
            clipboard.push_back(selectedPart);
            copiedText += selectedPart;

            if (i < endLine) {
                copiedText += "\n";
                clipboard.push_back("\n");
            }
        }

        SDL_SetClipboardText(copiedText.c_str()); // Save to system clipboard
    }
}
void pasting_from_clipboard() {
    char* clipboardText = SDL_GetClipboardText();

    if (!clipboardText) return; // Safety check

    string text = clipboardText;
    SDL_free(clipboardText); // Free clipboard memory

    if (saving) {
        name_place.insert(name_place.size(), text);
        return;
    }

    // Split text into lines by '\n'
    vector<string> pastedLines;
    size_t start = 0, end;
    while ((end = text.find('\n', start)) != string::npos) {
        pastedLines.push_back(text.substr(start, end - start-1));
        start = end + 1;
    }
    pastedLines.push_back(text.substr(start)); // Add last line

    if (pastedLines.empty()) return; // No text to paste

    // Insert text line-by-line
    lines[currentLine].insert(cursorPos, pastedLines[0]); // First part in current line
    cursorPos += pastedLines[0].size();

    if (pastedLines.size() > 1) {
        string remainingText = lines[currentLine].substr(cursorPos);
        lines[currentLine] = lines[currentLine].substr(0, cursorPos);

        // Insert new lines from clipboard
        for (size_t i = 1; i < pastedLines.size(); ++i) {
            lines.insert(lines.begin() + currentLine + 1, pastedLines[i]);
            currentLine++;
            cursorPos = pastedLines[i].size();
        }

        // Append remaining text from original line
        lines[currentLine] += remainingText;
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
    get_directory_tree(path, directoryTree);
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
                    for (const auto& file : filePositions) {
                        SDL_Rect rect = file.first;
                        std::string filename = file.second;

                        if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                            mouseY >= rect.y && mouseY <= rect.y + rect.h) {

                            if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".cpp") {
                                std::cout << "Opening file: " << filename << std::endl;
                                std::string content = readFile(path + filename);
                                lines= string_to_vector(content);
                                currentLine=0;
                                cursorPos = 0;
                                std::cout << content << std::endl;  // Display or copy
                            }
                        }
                    }
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
                        currentLine=lines.size()-1;
                        cursorPos=lines[lines.size()-1].size();
                        for (int i = lines.size()-1; i >=0; --i) {
                            for (int j = 0; j < lines[i].size(); j) {
                                cout<<i<<"        "<<lines[i].size()<<" "<<lines.size()<<"  "<<currentLine<< "   " << cursorPos<<endl;
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
                            }
                        }

                    }
                    if (mouse_place_checker(mouseX, mouseY, rect9) && file_button_clicked){
                        //save
                        saving = true;
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
                if(saving){
                    name_place.insert(name_place.size(),event.text.text);
                } else if(lining){
                    line_find.insert(line_find.size(),event.text.text);
                }
                else{
                    if (currentLine >= 0 && currentLine < (int)lines.size()) {
                        if(lines[currentLine].size()>110){if (currentLine >= 0 ) {
                                string remainder = lines[currentLine].substr(cursorPos);
                                lines[currentLine] = lines[currentLine].substr(0, cursorPos);
                                lines.insert(lines.begin() + currentLine + 1, remainder);
                                currentLine++;
                                cursorPos = 0;
                            }}
                        lines[currentLine].insert(cursorPos, event.text.text);
                        cursorPos++;
                        //                   if(event.text.text == "/" && last_char == '/' && last_line == currentLine)first_comment= true;
//                    if(last_line != currentLine ) first_comment = false;
//                    if(event.text.text == "*" && last_char == '/' && last_line == currentLine)second_comment = true;
//                   if(event.text.text == "/" && last_char == '*' && last_line == currentLine)second_comment= false;
                    }
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
                    if(saving){
                        name_place.erase(name_place.size()-1);
                    } else if(lining){
                        line_find.erase(line_find.size()-1);
                    }
                    else{if (selecting) {
                            // Ensure selectionStartLine is before selectionEndLine
                            int startLine = min(selectionStartLine, selectionEndLine);
                            int endLine = max(selectionStartLine, selectionEndLine);
                            int startPos = (selectionStartLine < selectionEndLine) ? selectionStartPos : selectionEndPos;
                            int endPos = (selectionStartLine < selectionEndLine) ? selectionEndPos : selectionStartPos;

                            if (startLine == endLine) {
                                // Single-line selection: Remove substring
                                lines[startLine].erase(startPos, endPos - startPos);
                            } else {
                                // Multi-line selection: Merge text after deletion
                                lines[startLine] = lines[startLine].substr(0, startPos) + lines[endLine].substr(endPos);
                                lines.erase(lines.begin() + startLine + 1, lines.begin() + endLine + 1);
                            }

                            // Move cursor to start of selection
                            currentLine = startLine;
                            cursorPos = startPos;

                            // Reset selection
                            selecting = false;
                            selectionStartLine = selectionEndLine = -1;
                            selectionStartPos = selectionEndPos = -1;
                        }else { // Normal backspace behavior
                            if (event.key.keysym.sym == SDLK_BACKSPACE) {
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
                    }
                } }else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2) {
                    if(lining){
                        cursorPos=0;
                        currentLine = stoi(line_find)-1;
                        lining= false;
                    }
                    else if(saving){
                        saving_file(name_place , vector_to_string(lines, currentLine));
                        directoryTree.clear();
                        get_directory_tree(path, directoryTree);
                        saving= false;
                    } else{
                        if (currentLine >= 0 && currentLine < (int)lines.size()) {
                            string remainder = lines[currentLine].substr(cursorPos);
                            lines[currentLine] = lines[currentLine].substr(0, cursorPos);
                            lines.insert(lines.begin() + currentLine + 1, remainder);
                            currentLine++;
                            cursorPos = 0;
                        }
                    }
                }if (event.key.keysym.sym == SDLK_LEFT) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {  // Shift + Left
                        if (!selecting) { // Start selection
                            selecting = true;
                            selectionStartLine = currentLine;
                            selectionStartPos = cursorPos;
                        }
                        if (cursorPos > 0) {
                            cursorPos--;
                        } else if (currentLine > 0) {
                            currentLine--;
                            cursorPos = lines[currentLine].size();
                        }
                        selectionEndLine = currentLine;
                        selectionEndPos = cursorPos;
                    } else { // Normal Left
                        selecting = false;
                        if (cursorPos > 0) {
                            cursorPos--;
                        } else if (currentLine > 0) {
                            currentLine--;
                            cursorPos = lines[currentLine].size();
                        }
                    }
                }else if (event.key.keysym.sym == SDLK_RIGHT) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {  // Shift + Right
                        if (!selecting) {
                            selecting = true;
                            selectionStartLine = currentLine;
                            selectionStartPos = cursorPos;
                        }
                        if (cursorPos < (int)lines[currentLine].size()) {
                            cursorPos++;
                        } else if (currentLine < (int)lines.size() - 1) {
                            currentLine++;
                            cursorPos = 0;
                        }
                        selectionEndLine = currentLine;
                        selectionEndPos = cursorPos;
                    } else { // Normal Right
                        selecting = false;
                        if (cursorPos < (int)lines[currentLine].size()) {
                            cursorPos++;
                        } else if (currentLine < (int)lines.size() - 1) {
                            currentLine++;
                            cursorPos = 0;
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_UP) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {  // Shift + Up
                        if (!selecting) {
                            selecting = true;
                            selectionEndLine = currentLine;
                            selectionEndPos = cursorPos;
                        }
                        if (currentLine > 0) {
                            currentLine--;
                            cursorPos = min(cursorPos, (int)lines[currentLine].size());
                        }
                        // Ensure the selection grabs the left part of the cursor
                        selectionStartLine = currentLine;
                        selectionStartPos = cursorPos;
                        // Selects from the start of the line
                    } else {  // Normal Up
                        selecting = false;
                        if (currentLine > 0) {
                            currentLine--;
                            cursorPos = min(cursorPos, (int)lines[currentLine].size());
                        }
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {  // Shift + Down
                        if (!selecting) {
                            selecting = true;
                            selectionStartLine = currentLine;
                            selectionStartPos = cursorPos;
                        }
                        if (currentLine < (int)lines.size() - 1) {
                            currentLine++;
                            cursorPos = min(cursorPos, (int)lines[currentLine].size());
                        }
                        selectionEndLine = currentLine;
                        selectionEndPos = cursorPos;
                    } else { // Normal Down
                        selecting = false;
                        if (currentLine < (int)lines.size() - 1) {
                            currentLine++;
                            cursorPos = min(cursorPos, (int)lines[currentLine].size());
                        }
                    }
                }
//saving system
                else if (event.key.keysym.sym == SDLK_g) {
                    if (SDL_GetModState() & KMOD_CTRL) {

                        lining = true;
                    }
                }
                else if (event.key.keysym.sym == SDLK_s) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        saving= true;
                    }
                }
                else if (event.key.keysym.sym == SDLK_a) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        selecting= true;
                        select_all_text();
                    }
                }
                else if (event.key.keysym.sym == SDLK_c) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        if(selecting){
                            copying_text_to_clipboard();
                        }

                        //cout << "Copied text:\n";
                        //for (const string& line : clipboard) cout << line << endl; // Debugging output
                    }
                }

                else if (event.key.keysym.sym == SDLK_v) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        pasting_from_clipboard();
                    }
                }
                else if (event.key.keysym.sym == SDLK_x) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        if (selecting) {
                            copying_text_to_clipboard();
                            // Ensure selectionStartLine is before selectionEndLine
                            int startLine = min(selectionStartLine, selectionEndLine);
                            int endLine = max(selectionStartLine, selectionEndLine);
                            int startPos = (selectionStartLine < selectionEndLine) ? selectionStartPos : selectionEndPos;
                            int endPos = (selectionStartLine < selectionEndLine) ? selectionEndPos : selectionStartPos;

                            if (startLine == endLine) {
                                // Single-line selection: Remove substring
                                lines[startLine].erase(startPos, endPos - startPos);
                            } else {
                                // Multi-line selection: Merge text after deletion
                                lines[startLine] = lines[startLine].substr(0, startPos) + lines[endLine].substr(endPos);
                                lines.erase(lines.begin() + startLine + 1, lines.begin() + endLine + 1);
                            }

                            // Move cursor to start of selection
                            currentLine = startLine;
                            cursorPos = startPos;

                            // Reset selection
                            selecting = false;
                            selectionStartLine = selectionEndLine = -1;
                            selectionStartPos = selectionEndPos = -1;
                        }


                        selected_text="";
                    }
                }
            }

        }

        if (SDL_GetTicks() - cursorTimer >= CURSOR_BLINK_INTERVAL) {
            cursorVisible = !cursorVisible;
            cursorTimer = SDL_GetTicks();
        }

        ensureLastLineVisible(currentLine, SCREEN_HEIGHT - 250, LINE_HEIGHT, lines.size(), SCREEN_WIDTH);
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
                SDL_Color highlightColor = {100, 100, 255, 100}; // Light Blue
                if (selecting && i >= min(selectionStartLine, selectionEndLine) &&
                    i <= max(selectionStartLine, selectionEndLine)) {

                    int startX = 0, endX = 0;
                    int start = (i == selectionStartLine) ? selectionStartPos : 0;
                    int end = (i == selectionEndLine) ? selectionEndPos : lines[i].size();

                    TTF_SizeText(font, lines[i].substr(0, start).c_str(), &startX, nullptr);
                    TTF_SizeText(font, lines[i].substr(0, end).c_str(), &endX, nullptr);

                    SDL_Rect highlightRect = {200 + startX - scrollOffsetX, y, endX - startX, LINE_HEIGHT};
                    SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
                    SDL_RenderFillRect(renderer, &highlightRect);
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

        if (directoryTree[0].size()<=1 && !abcd) {
            abcd= true;
            directoryTree.push_back("No files here");
        }
        renderText1(directoryTree);
        if(saving){
            SDL_SetRenderDrawColor(renderer, 140, 140, 140, 255);
            SDL_RenderFillRect(renderer, &save_name);
            int x =name_place.size();
            SDL_Rect hahahaha={500 , 500 , x*10 +1,50};
            renderText(name_place , hahahaha , green , font);
        }
        if(lining){
            SDL_SetRenderDrawColor(renderer, 140, 140, 140, 255);
            SDL_RenderFillRect(renderer, &save_name);
            int x =line_find.size();
            SDL_Rect hahahaha={500 , 500 , x*10 +1,50};
            renderText(line_find , hahahaha , green , font);
        }
       // cout<< currentLine << "  "<<currentLine << "   "<< cursorPos<< "    " << scrollOffset<<"   "<<scrollOffsetX<<endl;
        SDL_RenderPresent(renderer);
        iostream_library= false;
        cmath_library = false;
        bitsstdch = false;
        error_lines=0;

    }


    cleanup();
    return 0;
}