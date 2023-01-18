#include <ft2build.h>
#include <freetype.h>
#include "Model.hpp"


struct Character {
GLuint TextureID;  // ID handle of the glyph texture
glm::ivec2   Size;       // Size of glyph
glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;

unsigned int TextRenderVAO, TextRenderVBO;

void FontRenderSetup()
{
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

	// find path to font
    std::string font_name = "res/fonts/kongtext.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
    }

    
	
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            GLuint texture;

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D,
                        0,
                        GL_ALPHA,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_ALPHA,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void DrawText(std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4 cameraMatrix = glm::mat4(1.0f))
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glLoadMatrixf(glm::value_ptr(cameraMatrix));
    
    // iterate through all characters
    std::string::const_iterator c;
    unsigned int charindex = 0;
    float Originalx = x;
    float Originaly = y;
    bool skipchar = false;

    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        if (text[charindex] == '\n')
        {
            y -= (ch.Advance >> 6) * scale;
            x = Originalx;

            skipchar = true;
        }


        if(skipchar == false)
        {
            
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            if (text[charindex] != '\n')
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                glColor3f(color.x, color.y, color.z);
            }

            
            
            glBegin(GL_QUADS);
            //Front face
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(xpos, ypos + h);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(xpos, ypos);
            
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(xpos + w, ypos);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(xpos + w, ypos + h);
        
            glEnd();

            glDisable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, 0);
            
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }else{
            skipchar = false;
        }

        charindex++;
    }

    
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
}