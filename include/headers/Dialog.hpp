#include "FontRender.hpp"
#include "MainModels.hpp"
#include "Audio.hpp"

using namespace nlohmann;

int Frameindex = 1;
bool promptExpansion = false;

std::string PlanePath = "res/gfx/penguin.gltf";

std::vector <Model> PromptBubbleOutline;
std::vector <Model> PromptBubble;

std::string DialogTexture = "penguin.png";

int CharIndex = 0;
int DialogueFrameindex = 1;
int SpriteChangeIndex = 1;

bool PressedE = false;
bool DrawFirstFrame = true;

int dialogstringIndex = 0;

struct Dialog {
bool Talking;

int SpriteIndex;

std::vector <std::string> NPCtext;

std::string VoiceSound;

std::string DefaultSprite;
std::string TalkAnimationSprite1;
std::string TalkAnimationSprite2;
};

std::map<int, Dialog> Dialogs;

std::string GetDialogueSprite()
{
    return DialogTexture;
}

void SetupDialog(std::string file = "res/dialog/dialog.json")
{
    PromptBubbleOutline.push_back(Model((PlanePath).c_str(), glm::vec3(25.0f, 5.5f, 22.0f),glm::quat(-0.25f, 0.0f, 0.0f, 1.0f),glm::vec3(0.44f, 0.4f, 0.4f), "PromptBubbleOutline1.png"));


    std::string text = get_file_contents( (file).c_str() );
    json JSONDialog = json::parse(text);

    for(int c = 0; c <= JSONDialog["dialogues"].size() - 1; c++)
    {
        std::string voiceSound = JSONDialog["dialogues"][c]["VoiceSound"];
        std::string DefaultSprite = JSONDialog["dialogues"][c]["DefaultSprite"];
        std::string TalkAnimationSprite1 = JSONDialog["dialogues"][c]["TalkAnimationSprite1"];
        std::string TalkAnimationSprite2 = JSONDialog["dialogues"][c]["TalkAnimationSprite2"];
        std::vector <std::string> NPCtext = JSONDialog["dialogues"][c]["NPCtext"];

        Dialog dialog = {
            false,
        
            JSONDialog["dialogues"][c]["ModelIndex"],
        
            NPCtext,

            voiceSound,

            DefaultSprite,
            TalkAnimationSprite1,
            TalkAnimationSprite2
        };

        Dialogs.insert(std::pair<int, Dialog>(c, dialog));

        glm::vec3 ModelPos = GetModels()[dialog.SpriteIndex].GetPosition();



        PromptBubble.push_back(Model((PlanePath).c_str(), glm::vec3(ModelPos.x, ModelPos.y + 5.4f, ModelPos.z - 3.0f),glm::quat(-0.25f, 0.0f, 0.0f, 1.0f),glm::vec3(0.44f, 0.4f, 0.4f), "EPromptBubble.png"));
    } 
}

void GetDialogInput(bool EKey, glm::vec3 Position1)
{
    for(int i = 0; i <= Dialogs.size() - 1; i++)
    {
        std::vector <std::string> NPCtext = Dialogs[i].NPCtext;

        Model Sprite = GetModels()[Dialogs[i].SpriteIndex];
        
        glm::vec3 Position2 = Sprite.GetPosition();

        glm::vec3 ModelPos = GetModels()[Dialogs[i].SpriteIndex].GetPosition();


        //The distance from the first position and the second position 

        float VectorDistance = glm::distance(Position1, Position2);


        //check the key input

        if (EKey == true && VectorDistance <= 5)
        {

            if (Dialogs[i].Talking == true && VectorDistance <= 5 && CharIndex < NPCtext[dialogstringIndex].length() && PressedE == false)
            {
                CharIndex = NPCtext[dialogstringIndex].length();

                Dialogs[i].Talking = true;
            }else if(Dialogs[i].Talking == true && VectorDistance <= 5 && CharIndex >= NPCtext[dialogstringIndex].length() && PressedE == false)
            {
                CharIndex = 0;

                if(dialogstringIndex + 1 < NPCtext.size())
                {
                    dialogstringIndex++;
                }else{
                    Dialogs[i].Talking = false;
                    dialogstringIndex = 0;
                }

            }else if(PressedE == false)
            {
                Dialogs[i].Talking = true;
            }

            PressedE = true;

            //Move prompt bubble if the key and is close to the bubble


            if(PromptBubble[i].GetPosition() != glm::vec3(ModelPos.x, ModelPos.y + 5.4f, ModelPos.z - 3.0f) - glm::vec3(0.0f, 0.5f, 0.0f) && VectorDistance <= 5)
            {
                PromptBubble[i].Move(glm::vec3(ModelPos.x, ModelPos.y + 5.4f, ModelPos.z - 3.0f) - glm::vec3(0.0f, 0.3f, 0.0f), "EPromptBubble.png");
            }
        }else if(VectorDistance <= 5){
            PressedE = false;

            //Set bubble to default Position


            if(PromptBubble[i].GetPosition() != glm::vec3(ModelPos.x, ModelPos.y + 5.4f, ModelPos.z - 3.0f))
                PromptBubble[i].Move(glm::vec3(ModelPos.x, ModelPos.y + 5.4f, ModelPos.z - 3.0f), "EPromptBubble.png");
        }
    }
}

void RenderDialog(float fps, glm::mat4 textProjection, Camera camera, glm::vec3 Position1)
{
    Frameindex++;

    for(int i = 0; i <= Dialogs.size() - 1; i++)
    {
        std::vector <std::string> NPCtext = Dialogs[i].NPCtext;
        std::string TalkFrame1 = Dialogs[i].TalkAnimationSprite1;
        std::string TalkFrame2 = Dialogs[i].TalkAnimationSprite2;

        std::string DefaultFrame = Dialogs[i].DefaultSprite;

        std::string VoiceSound = Dialogs[i].VoiceSound;
        
        Model ModelSprite = GetModels()[Dialogs[i].SpriteIndex];
        
        glm::vec3 OtherPosition = ModelSprite.GetPosition();


        float VectorDistance = glm::distance(Position1, OtherPosition);

        

        if (Dialogs[i].Talking == true && VectorDistance <= 5)
        {
            //Create text animation

            DialogueFrameindex++;
            if ( CharIndex < NPCtext[dialogstringIndex].length() && fps / DialogueFrameindex < 11.5)
            {	
                DialogueFrameindex = 1;
                CharIndex++;
            }

            SpriteChangeIndex++;
            if ( CharIndex < NPCtext[dialogstringIndex].length() && fps / SpriteChangeIndex < 2.5 && DefaultFrame != "NULL")
            {
                SpriteChangeIndex = 1;

                PlaySFX(VoiceSound, 0, 0);
                
                if(DrawFirstFrame == true)
                {
                    DialogTexture = TalkFrame1;

                    DrawFirstFrame = false;
                }else{
                    DialogTexture = TalkFrame2;

                    DrawFirstFrame = true;
                }

                CurrentModels[Dialogs[i].SpriteIndex].ChangeTexture(DialogTexture);
                
            }else if ( CharIndex >= NPCtext[dialogstringIndex].length() && fps / SpriteChangeIndex < 2){
                SpriteChangeIndex = 1;

                DialogTexture = DefaultFrame;

                CurrentModels[Dialogs[i].SpriteIndex].ChangeTexture(DialogTexture);
            }

            //Draw the text itself

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);

            DrawText(NPCtext[dialogstringIndex].substr(0, CharIndex), 25.0f, 662.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f), textProjection);
            
        }else if(Dialogs[i].Talking == true){
            DialogTexture = DefaultFrame;

            CurrentModels[Dialogs[i].SpriteIndex].ChangeTexture(DialogTexture);
                
            DrawFirstFrame = true;
            
            CharIndex = 0;
            Dialogs[i].Talking = false;
            

            if(VectorDistance > 5)
            {
                dialogstringIndex = 0;
            }
        }



        if(Dialogs[i].Talking == false)
        {
            DialogTexture = DefaultFrame;

            CurrentModels[Dialogs[i].SpriteIndex].ChangeTexture(DialogTexture);
        }

        //Set the prompt bubble opacity

        if(VectorDistance <= 10 && PressedE == false)
        {
            if(VectorDistance <= 5)
            {
                PromptBubble[i].Alpha = 1.0f;
            }else
            {
                PromptBubble[i].Alpha = (1.0f - (0.05f * floor(VectorDistance - 5))) - 0.1f;
            }
        }else{
            if(VectorDistance <= 5 && PressedE == true || VectorDistance > 10)
            {
                PromptBubble[i].Alpha = 0.5f;
            }
        }

        //Draw the prompt bubble

        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        

        PromptBubble[i].Draw("Default", camera);



        if(fps / Frameindex < 2)
        {
            Frameindex = 1;
            if(promptExpansion == false)
            {
                promptExpansion = true;
            }else{
                promptExpansion = false;
            }
        }

        //Draw the prompt animation if its close to it

        if(VectorDistance <= 5 && Dialogs[i].Talking == false)
        {
            PromptBubbleOutline[0].Move(PromptBubble[i].GetPosition());

            if(promptExpansion == false)
            {
                PromptBubbleOutline[0].ChangeTexture("PromptBubbleOutline1.png");

                PromptBubbleOutline[0].Draw("Default", camera);
            }else{
                PromptBubbleOutline[0].ChangeTexture("PromptBubbleOutline2.png");

                PromptBubbleOutline[0].Draw("Default", camera);
            }
        } 
    }
}

void ClearDialogs()
{
    Dialogs.clear();

    PromptBubbleOutline.clear();

    PromptBubble.clear();
}