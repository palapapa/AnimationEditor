#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

#define  XY                       2
#define  XPOS                     0
#define  YPOS                     1
#define  MAX_X_COORDINATE         30
#define  MAX_Y_COORDINATE         30
#define  MAX_FRAME                256
#define  PREVIEW_OFFSET_X         1               //default
#define  PREVIEW_OFFSET_Y         10              //default
#define  PREVIEW_RULER_LENGTH     MAX_X_COORDINATE//default
#define  MAX_COODRINATE_SETS      2
#define  SET_0                    0               //for first set of coordinates
#define  SET_1                    1               //for second set of coordinates
#define  DEFAULT_STRING_LENGTH    128

#define  CLEAR_SCREEN             system("cls")
#define  RESET_CURSOR             gotoxy(0,0)
#define  PAUSE                    system("pause")
#define  SWAP(X,Y)                X=X^Y;\
                                  Y=Y^X;\
                                  X=X^Y
#define  SET_UP_OPTION_PARSER     char *cmdOptionTemp=user.cmdOption;/*declares a string literal for address arithmetic*/\
                                  char parsedOption[8];/*stores a parsed option substring*/\
                                  int offset=0;/*address offset*/\
                                  while(sscanf(cmdOptionTemp+=offset," %s%n",parsedOption,&offset)==1)

#define  LANG_PROMPT              ">"
#define  LANG_BAD_INPUT           "Bad input"
#define  LANG_CANT_DETECT_INPUT   "Can't detect input"
#define  LANG_UNKNOWN_COMMAND     "Unknown command"
#define  LANG_ARGUMENT_ERROR      "Arguments provided are insufficient or incorrect"
#define  LANG_NO_SUCH_FILE        "No save files with the specified name was found\nMake sure not to include the .txt extension"
#define  LANG_CANT_SAVE           "A error occured when trying to save\nPossibly because the file name contained illegal characters /\\:*?\"><|"
#define  LANG_CANT_LOAD           "A error occured when trying to load\nCheck if you spelled the save file name right"
#define  LANG_ILLEGAL_CHARACTER   "File name can't contain the following characters /\\:*?\"><|"
#define  LANG_WRONG_OPTION        "One or more options provided are incorrect(correct ones have been executed)"
#define  LANG_DESCRIPTION_SO      "Help: \"so X Y\" will set the offset of the current frame\n"
#define  LANG_DESCRIPTION_SPO     "Help: \"spo X Y\" will set the offset of the preview window\n"
#define  LANG_DESCRIPTION_SR      "Help: \"sr L\" will set the length of the rulers to L\n"
#define  LANG_DESCRIPTION_DEL     "Help: \"del X Y\" will delete the character on the (X,Y) coordinate of the current frame\n"
#define  LANG_DESCRIPTION_P       "Help: \"p X Y C\" will place a character C on the (X,Y) coordinate of the current frame\n"
#define  LANG_DESCRIPTION_NF      "Help: \"nf\" will create a new frame\n"
#define  LANG_DESCRIPTION_SD      "Help: \"sd MS\" will set the delay to the next frame to MS miliseconds\n"
#define  LANG_DESCRIPTION_TF      "Help: \"tf F\" will make the program jump to frame F(can't exceed number of frame created)\n"
#define  LANG_DESCRIPTION_FILL    "Help: \"fill X1 Y1 X2 Y2 C\" will fill the area specified with the character C\n"
#define  LANG_DESCRIPTION_DRANGE  "Help: \"drange X1 Y1 X2 Y2\" will remove the characters within the area specified"
#define  LANG_DESCRIPTION_OUTPUT  "Help: \"output\" will generate the code for your animation\n"
#define  LANG_DESCRIPTION_SAVE    "Help: \"save FILENAME\" will save the current project with a file name of FILENAME.txt\n"
#define  LANG_DESCRIPTION_LOAD    "Help: \"load FILENAME\" will load the save file with a name of T(without the .txt extension)"
#define  LANG_DESCRIPTION_COPY    "Help: \"copy F\" will copy the layout and settings of frame X to this frame\n"
#define  LANG_DESCRIPTION_PREVIEW "Help: \"preview T\" will show the animation in action for T times\n"
#define  LANG_DESCRIPTION_DELALL  "Help: \"delall\" will delete all characters of the current frame\n"
#define  LANG_DESCRIPTION_HELP    "Help: \"so X Y [-a]\" will set the offset of the current frame\n\
Help: \"spo X Y\" will set the offset of the preview window\n\
Help: \"sr L\" will set the length of the rulers to L\n\
Help: \"del X Y\" will delete the character on the (X,Y) coordinate of the current frame\n\
Help: \"p X Y C\" will place a character C on the (X,Y) coordinate of the current frame\n\
Help: \"nf\" will create a new frame\n\
Help: \"sd MS [-a]\" will set the delay to the next frame to MS miliseconds\n\
Help: \"tf F\" will make the program jump to frame F(can't exceed number of frames created)\n\
Help: \"fill X1 Y1 X2 Y2 C [-o]\" will fill the area specified with the character C\n\
Help: \"drange X1 Y1 X2 Y2\" will remove the characters within the area specified\n\
Help: \"output\" will generate the code for your animation\n\
Help: \"save FILENAME\" will create a svae file named FILENAME.txt\n\
Help: \"load FILENAME\" will load the save file FILENAME(without the .txt extension)\n\
Help: \"copy F\" will copy the layout and settings of frame F to this frame\n\
Help: \"preview T\" will show the animation in action for T times\n\
Help: \"delall\" will delete all characters of the current frame\n"

#define  COMMAND_HELP             "help"
#define  COMMAND_SO               "so"            //set offset
#define  COMMAND_SPO              "spo"           //set preview offset
#define  COMMAND_SR               "sr"            //set ruler
#define  COMMAND_DEL              "del"           //delete
#define  COMMAND_P                "p"             //particle
#define  COMMAND_NF               "nf"            //new frame
#define  COMMAND_SD               "sd"            //set delay
#define  COMMAND_TF               "tf"            //to frame
#define  COMMAND_FILL             "fill"
#define  COMMAND_DRANGE           "drange"        //delete range
#define  COMMAND_OUTPUT           "output"
#define  COMMAND_SAVE             "save"
#define  COMMAND_LOAD             "load"
#define  COMMAND_COPY             "copy"
#define  COMMAND_PREVIEW          "preview"
#define  COMMAND_DELALL           "delall"
#define  OPTION_ALL               "-a"            //(so, sd) specifies that the action will apply to all frames
#define  OPTION_OUTLINE           "-o"            //(fill) outline the specified area
#define  OPTION_UP                "-u"            //(str)
#define  OPTION_DOWN              "-d"            //(str)
#define  OPTION_LEFT              "-l"            //(str)
#define  OPTION_RIGHT             "-r"            //(str)

typedef struct
{
    int pos[XY];
    char p;//particle
}particle_t;

typedef struct
{
    int index;//index of current frame
    int offset[XY];
    int delay;
    particle_t particle[MAX_X_COORDINATE][MAX_Y_COORDINATE];
}frame_t;

typedef struct
{
    char input[DEFAULT_STRING_LENGTH];//original input
    char cmd[8];//processed command
    int cmdPos[XY][MAX_COODRINATE_SETS];//stores all coordinate related numbers
    int cmdNum;//stores argument unrelated to coordinates
    char cmdParticle;
    int cmdDelay;
    int cmdFrame;
    //bool inOutput;
    int currentFrame;
    int maxFrameEdited;//the highest index of all edited frames
    char commandFeedback[DEFAULT_STRING_LENGTH];
    char commandOutput[DEFAULT_STRING_LENGTH];//stores the code generated
    int previewOffset[XY];
    int previewRulerLength;
    char saveFileName[DEFAULT_STRING_LENGTH];//stores custom project name
    char cmdString[DEFAULT_STRING_LENGTH];//stores a string argument
    char cmdOption[DEFAULT_STRING_LENGTH];//stores the option substring
}user_t;//stores variables for command input

frame_t frame[MAX_FRAME] = { 0 };
user_t user = { 0 };

void wait(unsigned int mseconds);
void gotoxy(short xPos, short yPos);
void draw_ui();

int main()
{
    printf("Loading...\n");
    user.previewOffset[XPOS] = PREVIEW_OFFSET_X;
    user.previewOffset[YPOS] = PREVIEW_OFFSET_Y;
    user.previewRulerLength = PREVIEW_RULER_LENGTH;
    FILE *saveFileStream;
    for (int i = 0; i < MAX_FRAME; i++)
    {
        frame[i].index = i;//initialize all frame properties
        //offsets are already 0
        frame[i].delay = 200;//5fps
        for (int j = 0; j < MAX_X_COORDINATE; j++)
        {
            for (int k = 0; k < MAX_Y_COORDINATE; k++)
            {
                frame[i].particle[j][k].pos[XPOS] = j;//set all coordinates to match the grid
                frame[i].particle[j][k].pos[YPOS] = k;
                frame[i].particle[j][k].p = ' ';//initialize all particles to blank spaces
            }
        }
    }
    CLEAR_SCREEN;
    while (1)
    {
        draw_ui();
        memset(user.commandFeedback, 0, strlen(user.commandFeedback));//clear feedback string
        if (fgets(user.input, sizeof(user.input), stdin) != NULL)//successful input
        {
            user.input[strcspn(user.input, "\n") + 1] = 0;//strip tailing newline
            if (sscanf(user.input, " %s", user.cmd) == 1)//successfully detected input
            {
                //parse command
                if (strcmp(user.cmd, COMMAND_SO) == 0)//set offset of the current frame
                {
                    if (sscanf(user.input, " %*s %d %d %[^\n]\n", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], user.cmdOption) == 2)//correct input(no option)
                    {
                        frame[user.currentFrame].offset[XPOS] = user.cmdPos[XPOS][SET_0];
                        frame[user.currentFrame].offset[YPOS] = user.cmdPos[YPOS][SET_0];
                        sprintf(user.commandFeedback, "Offset of the current frame has been set to (%d,%d)", frame[user.currentFrame].offset[XPOS], frame[user.currentFrame].offset[YPOS]);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %d %[^\n]\n", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], user.cmdOption) == 3)//if contains option
                    {
                        SET_UP_OPTION_PARSER
                            //while
                        {
                            bool isOptionWrong = false;
                            if (strcmp(parsedOption,OPTION_ALL) == 0)//-a
                            {
                                for (int i = 0; i <= user.maxFrameEdited; i++)
                                {
                                    frame[i].offset[XPOS] = user.cmdPos[XPOS][SET_0];
                                    frame[i].offset[YPOS] = user.cmdPos[YPOS][SET_0];
                                }
                                if (isOptionWrong == false)
                                {
                                    sprintf(user.commandFeedback,"Offset of all frames have been set to (%d,%d)",frame[user.currentFrame].offset[XPOS],frame[user.currentFrame].offset[YPOS]);
                                }
                            }
                            else//wrong option string
                            {
                                sprintf(user.commandFeedback,LANG_WRONG_OPTION);
                                isOptionWrong = true;//fixes commandFeedback to LANG_WRONG_OPTION
                            }
                        }
                    }
                    else if (sscanf(user.input, " %*s %d %d %[^\n]\n", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], user.cmdOption) == 0)//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_SO);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %d %[^\n]\n", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], user.cmdOption) == 1)//only one coordinate
                    {
                        sprintf(user.commandFeedback, LANG_ARGUMENT_ERROR);
                        continue;
                    }
                }

                else if (strcmp(user.cmd, COMMAND_SPO) == 0)//set offset of preview window
                {
                    if (sscanf(user.input, " %*s %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0]) == 2)//correct input
                    {
                        user.previewOffset[XPOS] = user.cmdPos[XPOS][SET_0];
                        user.previewOffset[YPOS] = user.cmdPos[YPOS][SET_0];
                        sprintf(user.commandFeedback, "Offset of the preview window has been set to (%d,%d)", user.previewOffset[XPOS], user.previewOffset[YPOS]);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0]) == 0)//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_SPO);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0]) == 1)//only one coordinate
                    {
                        sprintf(user.commandFeedback, LANG_ARGUMENT_ERROR);
                        continue;
                    }
                }

                else if (strcmp(user.cmd, COMMAND_SR) == 0)//set the length of two preview rulers
                {
                    if (sscanf(user.input, " %*s %d", &user.cmdNum) == 1)//correct input
                    {
                        user.previewRulerLength = user.cmdNum;
                        sprintf(user.commandFeedback, "The length of ruler has been set to %d", user.previewRulerLength);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d", &user.cmdNum) == 0)//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_SR);
                        continue;
                    }
                }

                else if (strcmp(user.cmd, COMMAND_DEL) == 0)//delete particle on X Y
                {
                    if (sscanf(user.input, " %*s %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0]) == 2)//correct input
                    {
                        char temp = frame[user.currentFrame].particle[user.cmdPos[XPOS][SET_0]][user.cmdPos[YPOS][SET_0]].p;//used to display the deleted char
                        frame[user.currentFrame].particle[user.cmdPos[XPOS][SET_0]][user.cmdPos[YPOS][SET_0]].p = ' ';
                        sprintf(user.commandFeedback, "Deleted \'%c\' on (%d,%d)", temp, user.cmdPos[XPOS][SET_0], user.cmdPos[YPOS][SET_0]);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0]) == 1)//insufficient arguments
                    {
                        sprintf(user.commandFeedback, LANG_ARGUMENT_ERROR);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0]) == 0)//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_DEL);
                        continue;
                    }
                }

                else if (strcmp(user.cmd, COMMAND_P) == 0)//add particle on X Y
                {
                    if (sscanf(user.input, " %*s %d %d %c", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdParticle) == 3)//correct input
                    {
                        if (user.cmdPos[XPOS][SET_0] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_0] > MAX_Y_COORDINATE)//tried to place a char beyond the limit
                        {
                            sprintf(user.commandFeedback, "Can't add a character beyond (%d,%d)", MAX_X_COORDINATE, MAX_Y_COORDINATE);
                            continue;
                        }
                        else if (user.cmdPos[XPOS][SET_0] < 0 || user.cmdPos[YPOS][SET_0] < 0)//tried to place a character on a coordinate with negative x or y
                        {
                            sprintf(user.commandFeedback, "Can't add a character on a coordinate with negative x or y");
                        }
                        else
                        {
                            frame[user.currentFrame].particle[user.cmdPos[XPOS][SET_0]][user.cmdPos[YPOS][SET_0]].p = user.cmdParticle;
                            sprintf(user.commandFeedback, "\'%c\' has been added on (%d,%d)", user.cmdParticle, user.cmdPos[XPOS][SET_0], user.cmdPos[YPOS][SET_0]);
                            continue;
                        }
                    }
                    else if (sscanf(user.input, " %*s %d %d %c", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdParticle) < 3)//incorrect input
                    {
                        if (sscanf(user.input, " %*s %d %d %c", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdParticle) == 0)//only the command keyword
                        {
                            sprintf(user.commandFeedback, LANG_DESCRIPTION_P);
                            continue;
                        }
                        else//insufficient arguments
                        {
                            sprintf(user.commandFeedback, LANG_ARGUMENT_ERROR);
                            continue;
                        }
                    }
                }

                else if (strcmp(user.cmd, COMMAND_NF) == 0)//create a new frame
                {
                    if (user.maxFrameEdited >= MAX_FRAME - 1)//if already reached frame count limit
                    {
                        sprintf(user.commandFeedback, "Max frame count(%d) reached", MAX_FRAME);
                    }
                    else
                    {
                        user.maxFrameEdited++;
                        user.currentFrame = user.maxFrameEdited;//jump to the frame just created
                        sprintf(user.commandFeedback, "Created a new frame");
                    }
                }

                else if (strcmp(user.cmd, COMMAND_SD) == 0)//set delay to the next frame
                {
                    if (sscanf(user.input, " %*s %d %[^\n]\n", &user.cmdNum, user.cmdOption) == 1)//correct input(no option)
                    {
                        frame[user.currentFrame].delay = user.cmdNum;
                        sprintf(user.commandFeedback, "Delay to the next frame has been set to %dms", user.cmdNum);
                        continue;
                    }
                    else if (sscanf(user.input, " %*s %d %[^\n]\n", &user.cmdNum, user.cmdOption) == 2)//with option
                    {
                        SET_UP_OPTION_PARSER
                        {
                            bool isOptionWrong = false;
                            if (strcmp(parsedOption,OPTION_ALL) == 0)//-a
                            {
                                for (int i = 0; i <= user.maxFrameEdited; i++)
                                {
                                    frame[i].delay = user.cmdNum;
                                }
                                if (isOptionWrong == false)
                                {
                                    sprintf(user.commandFeedback,"Delay of all frames have been set to %dms",user.cmdNum);
                                }
                            }
                            else//wrong option
                            {
                                sprintf(user.commandFeedback,LANG_WRONG_OPTION);
                                isOptionWrong = true;//fixes commandFeedback to LANG_WRONG_OPTION
                            }
                        }
                    }
                    else if (sscanf(user.input, " %*s %d %[^\n]\n", &user.cmdNum, user.cmdOption) == 0)//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_SD);
                        continue;
                    }
                }

                else if (strcmp(user.cmd, COMMAND_TF) == 0)//jump to frame F
                {
                    if (sscanf(user.input, " %*s %d", &user.cmdNum) == 1)//correct input
                    {
                        if (user.cmdNum > user.maxFrameEdited)//tried to jump beyond the max frame edited
                        {
                            sprintf(user.commandFeedback, "Can't jump beyond the last frame you edited");
                            continue;
                        }
                        else//correct argument
                        {
                            user.currentFrame = user.cmdNum;
                            sprintf(user.commandFeedback, "Jumped to frame %d", user.cmdNum);
                            continue;
                        }
                    }
                }

                else if (strcmp(user.cmd, COMMAND_FILL) == 0)//fill area with character
                {
                    if (sscanf(user.input, " %*s %d %d %d %d %c %[^\n]\n", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1], &user.cmdParticle, user.cmdOption) == 5)//correct input(no option)
                    {
                        if (user.cmdPos[XPOS][SET_0] < 0 || user.cmdPos[YPOS][SET_0] < 0 || user.cmdPos[XPOS][SET_1] < 0 || user.cmdPos[YPOS][SET_1] < 0)//tried to place a character on a coordinate with negative x or y
                        {
                            sprintf(user.commandFeedback, "Can't add a character on a coordinate with negative x or y");
                        }
                        else if (user.cmdPos[XPOS][SET_0] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_0] > MAX_Y_COORDINATE || user.cmdPos[XPOS][SET_1] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_1] > MAX_Y_COORDINATE)//if out of bounds
                        {
                            sprintf(user.commandFeedback, "Can't add a character beyond (%d,%d)", MAX_X_COORDINATE, MAX_Y_COORDINATE);
                        }
                        //else if(user.cmdPos[XPOS][SET_0]!=user.cmdPos[XPOS][SET_1]&&user.cmdPos[YPOS][SET_0]!=user.cmdPos[YPOS][SET_1])//x1!=x2 and y1!=y2
                        else//all correct
                        {
                            if (user.cmdPos[XPOS][SET_0] > user.cmdPos[XPOS][SET_1])//x1>x2
                            {
                                SWAP(user.cmdPos[XPOS][SET_0], user.cmdPos[XPOS][SET_1]);//now: x1<x2
                            }
                            if (user.cmdPos[YPOS][SET_0] > user.cmdPos[YPOS][SET_1])//y1>y2
                            {
                                SWAP(user.cmdPos[YPOS][SET_0], user.cmdPos[YPOS][SET_1]);//now: y1<y2
                            }
                            //guarantee that x1<x2 and y1<y2
                            for (int i = user.cmdPos[XPOS][SET_0]; i <= user.cmdPos[XPOS][SET_1]; i++)//from x1 to x2(horizontal)
                            {
                                for (int j = user.cmdPos[YPOS][SET_0]; j <= user.cmdPos[YPOS][SET_1]; j++)//from y1 to y2(vertical)
                                {
                                    frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                                }
                            }
                            sprintf(user.commandFeedback, "Filled %d \'%c\'", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]), user.cmdParticle);
                        }
#if 0
                    else if (user.cmdPos[XPOS][SET_0] == user.cmdPos[XPOS][SET_1])//x1=x2(vertical line)
                    {
                        if (user.cmdPos[XPOS][SET_0] > user.cmdPos[XPOS][SET_1])//x1>x2
                        {
                            SWAP(user.cmdPos[XPOS][SET_0], user.cmdPos[XPOS][SET_1]);//now: x1<x2
                        }
                        for (int j = user.cmdPos[YPOS][SET_0]; j <= user.cmdPos[YPOS][SET_1]; j++)//from y1 to y2(vertical)
                        {
                            int i = user.cmdPos[XPOS][SET_0];
                            frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                        }
                        sprintf(user.commandFeedback, "Filled %d \'%c\'", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]), user.cmdParticle);
                    }
                    else if (user.cmdPos[YPOS][SET_0] == user.cmdPos[YPOS][SET_1])//y1=y2(horizontal line)
                    {
                        if (user.cmdPos[YPOS][SET_0] > user.cmdPos[YPOS][SET_1])//x1>x2
                        {
                            SWAP(user.cmdPos[YPOS][SET_0], user.cmdPos[YPOS][SET_1]);//now: x1<x2
                        }
                        for (int i = user.cmdPos[XPOS][SET_0]; i <= user.cmdPos[XPOS][SET_1]; i++)//from x1 to x2(horizontal)
                        {
                            int j = user.cmdPos[YPOS][SET_0];
                            frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                        }
                        sprintf(user.commandFeedback, "Filled %d \'%c\'", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]), user.cmdParticle);
                    }
                    else if (user.cmdPos[XPOS][SET_0] == user.cmdPos[XPOS][SET_1] && user.cmdPos[YPOS][SET_0] == user.cmdPos[YPOS][SET_1])//x1=x2 and y1=y2
                    {
                        frame[user.currentFrame].particle[user.cmdPos[XPOS][SET_0]][user.cmdPos[YPOS][SET_0]].p = user.cmdParticle;
                        sprintf(user.commandFeedback, "Filled %d \'%c\'", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]), user.cmdParticle);
                    }
                    continue;
#endif
                    }
                    else if (sscanf(user.input, " %*s %d %d %d %d %c %[^\n]\n", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1], &user.cmdParticle, user.cmdOption) == 6)//with option
                    {
                        SET_UP_OPTION_PARSER
                        {
                            bool isOptionWrong = false;
                            if (strcmp(parsedOption,OPTION_OUTLINE) == 0)//-o
                            {
                                if (user.cmdPos[XPOS][SET_0] < 0 || user.cmdPos[YPOS][SET_0] < 0 || user.cmdPos[XPOS][SET_1] < 0 || user.cmdPos[YPOS][SET_1] < 0)//tried to place a character on a coordinate with negative x or y
                                {
                                    sprintf(user.commandFeedback,"Can't add a character on a coordinate with negative x or y");
                                }
                                else if (user.cmdPos[XPOS][SET_0] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_0] > MAX_Y_COORDINATE || user.cmdPos[XPOS][SET_1] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_1] > MAX_Y_COORDINATE)//if out of bounds
                                {
                                    sprintf(user.commandFeedback,"Can't add a character beyond (%d,%d)",MAX_X_COORDINATE,MAX_Y_COORDINATE);
                                }
                                else//all correct
                                {
                                    if (user.cmdPos[XPOS][SET_0] > user.cmdPos[XPOS][SET_1])//x1>x2
                                    {
                                        SWAP(user.cmdPos[XPOS][SET_0],user.cmdPos[XPOS][SET_1]);//now: x1<x2
                                    }
                                    if (user.cmdPos[YPOS][SET_0] > user.cmdPos[YPOS][SET_1])//y1>y2
                                    {
                                        SWAP(user.cmdPos[YPOS][SET_0],user.cmdPos[YPOS][SET_1]);//now: y1<y2
                                    }
                                    //guarantee that x1<x2 and y1<y2
                                    for (int i = user.cmdPos[XPOS][SET_0]; i <= user.cmdPos[XPOS][SET_1]; i++)//from x1 to x2(horizontal)
                                    {
                                        int j = user.cmdPos[YPOS][SET_0];
                                        frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                                        j = user.cmdPos[YPOS][SET_1];
                                        frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                                    }
                                    for (int j = user.cmdPos[YPOS][SET_0]; j <= user.cmdPos[YPOS][SET_1]; j++)//from y1 to y2(vertical)
                                    {
                                        int i = user.cmdPos[XPOS][SET_0];
                                        frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                                        i = user.cmdPos[XPOS][SET_1];
                                        frame[user.currentFrame].particle[i][j].p = user.cmdParticle;
                                    }
                                    if (isOptionWrong == false)
                                    {
                                        sprintf(user.commandFeedback,"Outlined area between (%d,%d), (%d,%d)",user.cmdPos[XPOS][SET_0],user.cmdPos[YPOS][SET_0],user.cmdPos[XPOS][SET_1],user.cmdPos[YPOS][SET_1]);
                                    }
                                }
                            }
                            else//wrong option string
                            {
                                sprintf(user.commandFeedback,LANG_WRONG_OPTION);
                                isOptionWrong = true;
                            }
                        }
                    }
                    else//incorrect input
                    {
                        if (sscanf(user.input, " %*s %d %d %d %d %c", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1], &user.cmdParticle) < 5)
                        {
                            if (sscanf(user.input, " %*s %d %d %d %d %c", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1], &user.cmdParticle) == 0)//only the command keyword
                            {
                                sprintf(user.commandFeedback, LANG_DESCRIPTION_FILL);
                                continue;
                            }
                            else//insufficient arguments
                            {
                                sprintf(user.commandFeedback, LANG_ARGUMENT_ERROR);
                                continue;
                            }
                        }
                    }
                }

#if 0
                else if (strcmp(user.cmd, COMMAND_HELP) == 0)//show all commands
                {
                    sprintf(user.commandFeedback, LANG_DESCRIPTION_HELP);
                }
#endif

                else if (strcmp(user.cmd, COMMAND_DRANGE) == 0)//delete range
                {
                    if (sscanf(user.input, " %*s %d %d %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1]) == 4)//correct input
                    {
                        if (user.cmdPos[XPOS][SET_0] < 0 || user.cmdPos[YPOS][SET_0] < 0 || user.cmdPos[XPOS][SET_1] < 0 || user.cmdPos[YPOS][SET_1] < 0)//tried to place a character on a coordinate with negative x or y
                        {
                            sprintf(user.commandFeedback, "Can't delete a character on a coordinate with negative x or y");
                        }
                        else if (user.cmdPos[XPOS][SET_0] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_0] > MAX_Y_COORDINATE || user.cmdPos[XPOS][SET_1] > MAX_X_COORDINATE || user.cmdPos[YPOS][SET_1] > MAX_Y_COORDINATE)//if out of bounds
                        {
                            sprintf(user.commandFeedback, "Can't delete a character beyond (%d,%d)", MAX_X_COORDINATE, MAX_Y_COORDINATE);
                        }
                        else//correct input
                        {
                            if (user.cmdPos[XPOS][SET_0] > user.cmdPos[XPOS][SET_1])//x1>x2
                            {
                                SWAP(user.cmdPos[XPOS][SET_0], user.cmdPos[XPOS][SET_1]);//now: x1<x2
                            }
                            if (user.cmdPos[YPOS][SET_0] > user.cmdPos[YPOS][SET_1])//y1>y2
                            {
                                SWAP(user.cmdPos[YPOS][SET_0], user.cmdPos[YPOS][SET_1]);//now: y1<y2
                            }
                            //guarantee that x1<x2 and y1<y2
                            for (int i = user.cmdPos[XPOS][SET_0]; i <= user.cmdPos[XPOS][SET_1]; i++)//from x1 to x2(horizontal)
                            {
                                for (int j = user.cmdPos[YPOS][SET_0]; j <= user.cmdPos[YPOS][SET_1]; j++)//from y1 to y2(vertical)
                                {
                                    frame[user.currentFrame].particle[i][j].p = ' ';
                                }
                            }
                            sprintf(user.commandFeedback, "Deleted %d characters", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]));
                        }
#if 0
                    else if (user.cmdPos[XPOS][SET_0] == user.cmdPos[XPOS][SET_1])//x1=x2(vertical line)
                    {
                        if (user.cmdPos[XPOS][SET_0] > user.cmdPos[XPOS][SET_1])//x1>x2
                        {
                            SWAP(user.cmdPos[XPOS][SET_0], user.cmdPos[XPOS][SET_1]);//now: x1<x2
                        }
                        for (int j = user.cmdPos[YPOS][SET_0]; j <= user.cmdPos[YPOS][SET_1]; j++)//from y1 to y2(vertical)
                        {
                            int i = user.cmdPos[XPOS][SET_0];
                            frame[user.currentFrame].particle[i][j].p = ' ';
                        }
                        sprintf(user.commandFeedback, "Deleted %d characters", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]));
                    }
                    else if (user.cmdPos[YPOS][SET_0] == user.cmdPos[YPOS][SET_1])//y1=y2(horizontal line)
                    {
                        if (user.cmdPos[XPOS][SET_0] > user.cmdPos[XPOS][SET_1])//x1>x2
                        {
                            SWAP(user.cmdPos[XPOS][SET_0], user.cmdPos[XPOS][SET_1]);//now: x1<x2
                        }
                        for (int i = user.cmdPos[XPOS][SET_0]; i <= user.cmdPos[XPOS][SET_1]; i++)//from x1 to x2(horizontal)
                        {
                            int j = user.cmdPos[YPOS][SET_0];
                            frame[user.currentFrame].particle[i][j].p = ' ';
                        }
                        sprintf(user.commandFeedback, "Deleted %d characters", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]));
                    }
                    else if (user.cmdPos[XPOS][SET_0] == user.cmdPos[XPOS][SET_1] && user.cmdPos[YPOS][SET_0] == user.cmdPos[YPOS][SET_1])//x1=x2 and y1=y2
                    {
                        frame[user.currentFrame].particle[user.cmdPos[XPOS][SET_0]][user.cmdPos[YPOS][SET_0]].p = user.cmdParticle;
                        sprintf(user.commandFeedback, "Deleted %d characters", ((user.cmdPos[XPOS][SET_1] + 1) - user.cmdPos[XPOS][SET_0]) * ((user.cmdPos[YPOS][SET_1] + 1) - user.cmdPos[YPOS][SET_0]));
                    }
#endif
                    }
                    else//incorrect input
                    {
                        if (sscanf(user.input, " %*s %d %d %d %d", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1]) < 4)
                        {
                            if (sscanf(user.input, " %*s %d %d %d %d %c", &user.cmdPos[XPOS][SET_0], &user.cmdPos[YPOS][SET_0], &user.cmdPos[XPOS][SET_1], &user.cmdPos[YPOS][SET_1], &user.cmdParticle) == 0)//only the command keyword
                            {
                                sprintf(user.commandFeedback, LANG_DESCRIPTION_DRANGE);
                                continue;
                            }
                            else//insufficient arguments
                            {
                                sprintf(user.commandFeedback, LANG_ARGUMENT_ERROR);
                                continue;
                            }
                        }
                    }
                }

                else if (strcmp(user.cmd, COMMAND_OUTPUT) == 0)//output all code generated
                {
                    CLEAR_SCREEN;
                    for (int i = 0; i <= user.maxFrameEdited; i++)
                    {
                        for (int x = 0; x < MAX_X_COORDINATE; x++)
                        {
                            for (int y = 0; y < MAX_Y_COORDINATE; y++)
                            {
                                if (frame[i].particle[x][y].p != ' ')//if character on (x,y) is not a space
                                {
                                    printf("gotoxy(%d,%d);\n", x + frame[i].offset[XPOS], y + frame[i].offset[YPOS]);
                                    printf("printf(\"%c\");\n", frame[i].particle[x][y].p);
                                }
                            }
                        }
                        //after a frame is fully dealt with...
                        printf("wait(%d);\n", frame[i].delay);
                        printf("system(\"cls\");\n");
                    }
                    //printf("gotoxy(0,0);\n");
                    PAUSE;
                }

                else if (strcmp(user.cmd, COMMAND_SAVE) == 0)//save the current project
                {
                    if (sscanf(user.input, " %*s %s", user.cmdString) == 1)//correct input
                    {
                        //char fullFileName[DEFAULT_STRING_LENGTH+4]={0};//+4 to store the extra ".txt"
                        strcat(user.cmdString, ".txt");
                        saveFileStream = fopen(user.cmdString, "w");//creates a file for writing
                        if (saveFileStream == NULL)//failed to create file
                        {
                            sprintf(user.commandFeedback, LANG_CANT_SAVE);
                            continue;
                        }
                        else//save file created successfully
                        {
                            for (int i = 0; i < strlen(user.cmdString); i++)//check for illegal characters
                            {
                                if (strchr(user.cmdString, '/') != NULL ||
                                    strchr(user.cmdString, '\\') != NULL ||
                                    strchr(user.cmdString, ':') != NULL ||
                                    strchr(user.cmdString, '*') != NULL ||
                                    strchr(user.cmdString, '?') != NULL ||
                                    strchr(user.cmdString, '\"') != NULL ||
                                    strchr(user.cmdString, '>') != NULL ||
                                    strchr(user.cmdString, '<') != NULL ||
                                    strchr(user.cmdString, '|') != NULL
                                    )//if the input contains illegal characters
                                {
                                    sprintf(user.commandFeedback, LANG_ILLEGAL_CHARACTER);
                                    continue;
                                }
                            }
                            for (int frames = 0; frames < MAX_FRAME; frames++)
                            {
                                fprintf(saveFileStream, "frame=%d\nxOffset=%d\nyOffset=%d\ndelay=%d\n", frames, frame[frames].offset[XPOS], frame[frames].offset[YPOS], frame[frames].delay);//saves frame properties
                                for (int xPos = 0; xPos < MAX_X_COORDINATE; xPos++)
                                {
                                    for (int yPos = 0; yPos < MAX_Y_COORDINATE; yPos++)
                                    {
                                        if (frame[frames].particle[xPos][yPos].p != ' ')//skips blank characters
                                        {
                                            fprintf(saveFileStream, "xPos=%d\nyPos=%d\nparticle=%c\n", xPos, yPos, frame[frames].particle[xPos][yPos].p);//saves particles
                                        }
                                    }
                                }
                            }
                            fprintf(saveFileStream, "maxFrameEdited=%d\n", user.maxFrameEdited);//saves maxFrameEdited

                            sprintf(user.commandFeedback, "Save file \"%s\" has been created", user.cmdString);
                            fclose(saveFileStream);
                            continue;
                        }
                    }
                    else//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_SAVE);
                    }
                }

                else if (strcmp(user.cmd, COMMAND_LOAD) == 0)//load save file
                {
                    if (sscanf(user.input, " %*s %s", user.cmdString) == 1)//correct input
                    {
                        strcat(user.cmdString, ".txt");
                        saveFileStream = fopen(user.cmdString, "r");
                        if (saveFileStream == NULL)//failed to load file
                        {
                            sprintf(user.commandFeedback, LANG_CANT_LOAD);
                            continue;
                        }
                        else//save file loaded successfully
                        {
                            for (int i = 0; i < MAX_FRAME; i++)//resets all frames
                            {
                                frame[i].offset[XPOS] = 0;
                                frame[i].offset[YPOS] = 0;
                                frame[i].delay = 200;//5fps
                                for (int j = 0; j < MAX_X_COORDINATE; j++)
                                {
                                    for (int k = 0; k < MAX_Y_COORDINATE; k++)
                                    {
                                        frame[i].particle[j][k].p = ' ';//initialize all particles to blank spaces
                                    }
                                }
                            }
                            char parsedLine[128] = { 0 };//temporarily stores a whole line
                            char parsedKeyword[32] = { 0 };//temporarily stores a parsed keyword(frame, xPos etc)
                            int parsedNum = 0;//temporarily stores a parsed number
                            char parsedChar = 0;//temporarily stores a parsed char
                            int parsedFrame = 0;//temporarily stores an index of frame
                            int parsedXPos = 0;//temporarily stores a parsed xPos argument
                            int parsedYPos = 0;//temporarily stores a parsed yPos argument
                            while (feof(saveFileStream) == 0)//while EOF isn't reached
                            {
                                if (fgets(parsedLine, 128, saveFileStream) != NULL)
                                {
                                    if (sscanf(parsedLine, " %[^=]=%d", parsedKeyword, &parsedNum) == 2)//if fits pattern "Keyword=int Argument"
                                    {
                                        //stops working here
                                        if (strcmp(parsedKeyword, "frame") == 0)//"frame" keyword specifies the frame to assign properties to
                                        {
                                            parsedFrame = parsedNum;
                                        }
                                        else if (strcmp(parsedKeyword, "xOffset") == 0)//"xOffset" keyword specifies the x-axis offset of the frame specified
                                        {
                                            frame[parsedFrame].offset[XPOS] = parsedNum;
                                        }
                                        else if (strcmp(parsedKeyword, "yOffset") == 0)//"yOffset" keyword specifies the y-axis offset of the frame specified
                                        {
                                            frame[parsedFrame].offset[YPOS] = parsedNum;
                                        }
                                        else if (strcmp(parsedKeyword, "delay") == 0)//"delay" keyword specifies the delay of the frame specified
                                        {
                                            frame[parsedFrame].delay = parsedNum;
                                        }
                                        else if (strcmp(parsedKeyword, "xPos") == 0)//"xPos" keyword specifies the x coordinate to do action to
                                        {
                                            parsedXPos = parsedNum;
                                        }
                                        else if (strcmp(parsedKeyword, "yPos") == 0)//"yPos" keyword specifies the y coordinate to do action to
                                        {
                                            parsedYPos = parsedNum;
                                        }
                                        else if (strcmp(parsedKeyword, "maxFrameEdited") == 0)//"maxFrameEdited" specifies the value of maxFrameEdited
                                        {
                                            user.maxFrameEdited = parsedNum;
                                        }
                                    }
                                    else if (sscanf(parsedLine, " %[^=]=%c", parsedKeyword, &parsedChar) == 2)//if fits pattern "Keyword=char Argument"
                                    {
                                        if (strcmp(parsedKeyword, "particle") == 0)//"particle" keyword specifies the particle to add to (parsedXPos,parsedYPos)
                                        {
                                            frame[parsedFrame].particle[parsedXPos][parsedYPos].p = parsedChar;
                                        }
                                    }
                                    else//if fits no pattern
                                    {
                                        continue;
                                    }
                                }
                            }
                            sprintf(user.commandFeedback, "Loaded save file %s", user.cmdString);
                            fclose(saveFileStream);
                        }
                    }
                    else//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_SAVE);
                    }
                }

                else if (strcmp(user.cmd, COMMAND_COPY) == 0)//copy layout and settings to the current frame
                {
                    if (sscanf(user.input, " %*s %d", &user.cmdNum) == 1)//correct input
                    {
                        if (user.cmdNum > user.maxFrameEdited)//tried to copy an frame that doesn't exist
                        {
                            sprintf(user.commandFeedback, "Can't copy an frame that doesn't exist");
                            continue;
                        }
                        for (int i = 0; i < MAX_X_COORDINATE; i++)
                        {
                            for (int j = 0; j < MAX_Y_COORDINATE; j++)
                            {
                                frame[user.currentFrame].particle[i][j].p = ' ';//clear all characters
                            }
                        }
                        for (int i = 0; i < MAX_X_COORDINATE; i++)
                        {
                            for (int j = 0; j < MAX_Y_COORDINATE; j++)
                            {
                                frame[user.currentFrame].particle[i][j].p = frame[user.cmdNum].particle[i][j].p;//copy all characters
                                frame[user.currentFrame].delay = frame[user.cmdNum].delay;
                                frame[user.currentFrame].offset[XPOS] = frame[user.cmdNum].offset[XPOS];
                                frame[user.currentFrame].offset[YPOS] = frame[user.cmdNum].offset[YPOS];
                            }
                        }
                        sprintf(user.commandFeedback, "Copied frame %d to this frame", user.cmdNum);
                    }
                    else//only the command keyword
                    {
                        sprintf(user.commandFeedback, LANG_DESCRIPTION_COPY);
                        continue;
                    }
                }

                else if (strcmp(user.cmd, COMMAND_PREVIEW) == 0)//view the animation in action
                {
                    if (sscanf(user.input, " %*s %d", &user.cmdNum) == 1)//if contains loop count argument
                    {
                        for (int loops = 0; loops < user.cmdNum; loops++)
                        {
                            CLEAR_SCREEN;
                            for (int i = 0; i <= user.maxFrameEdited; i++)
                            {
                                CLEAR_SCREEN;
                                for (int x = 0; x < MAX_X_COORDINATE; x++)
                                {
                                    for (int y = 0; y < MAX_Y_COORDINATE; y++)
                                    {
                                        if (frame[i].particle[x][y].p != ' ')//if character on (x,y) is not a space
                                        {
                                            gotoxy(frame[i].particle[x][y].pos[XPOS], frame[i].particle[x][y].pos[YPOS]);
                                            printf("%c", frame[i].particle[x][y].p);
                                        }
                                    }
                                }
                                //after a frame is fully displayed...
                                wait(frame[i].delay);
                            }
                            gotoxy(0, MAX_Y_COORDINATE + 1);
                        }
                        system("pause");
                    }
                    else//only the command keyword
                    {
                        CLEAR_SCREEN;
                        for (int i = 0; i <= user.maxFrameEdited; i++)
                        {
                            CLEAR_SCREEN;
                            for (int x = 0; x < MAX_X_COORDINATE; x++)
                            {
                                for (int y = 0; y < MAX_Y_COORDINATE; y++)
                                {
                                    if (frame[i].particle[x][y].p != ' ')//if character on (x,y) is not a space
                                    {
                                        gotoxy(frame[i].particle[x][y].pos[XPOS], frame[i].particle[x][y].pos[YPOS]);
                                        printf("%c", frame[i].particle[x][y].p);
                                    }
                                }
                            }
                            //after a frame is fully displayed...
                            wait(frame[i].delay);
                        }
                        gotoxy(0, MAX_Y_COORDINATE + 1);
                        system("pause");
                    }
                }

                else if (strcmp(user.cmd, COMMAND_DELALL) == 0)//delete all characters of the current frame
                {
                    for (int j = 0; j < MAX_X_COORDINATE; j++)
                    {
                        for (int k = 0; k < MAX_Y_COORDINATE; k++)
                        {
                            frame[user.currentFrame].particle[j][k].p = ' ';
                        }
                    }
                    sprintf(user.commandFeedback, "Removed all characters of this frame");
                }

                else//input doesn't contain any available command
                {
                    sprintf(user.commandFeedback, LANG_UNKNOWN_COMMAND);
                    continue;
                }
            }
            else//can't detect command
            {
                sprintf(user.commandFeedback, LANG_CANT_DETECT_INPUT);
                continue;
            }
        }
        else//unsuccessful input
        {
            sprintf(user.commandFeedback, LANG_BAD_INPUT);
            continue;
        }
    }
    return 0;
}

void wait(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void gotoxy(short xPos, short yPos)
{
    COORD pos = { xPos,yPos };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void draw_ui()
{
    CLEAR_SCREEN;
    for (int i = 0; i < user.previewRulerLength; i++)
    {
        gotoxy(user.previewOffset[XPOS] + i, user.previewOffset[YPOS] - 1);//draw horizontal ruler
        printf("%d", i % 10);//this allows the length to be over 10
        gotoxy(user.previewOffset[XPOS] - 1, user.previewOffset[YPOS] + i);//draw vertical ruler
        printf("%d", i % 10);//this allows the length to be over 10
    }
#if 1
    for (int x = user.previewOffset[XPOS], i = 0; x < MAX_X_COORDINATE + user.previewOffset[XPOS]; x++, i++)//draw preview //i is used for indexing
    {
        for (int y = user.previewOffset[YPOS], j = 0; y < MAX_Y_COORDINATE + user.previewOffset[YPOS]; y++, j++)//j is used for indexing
        {
            gotoxy(x, y);
            printf("%c", frame[user.currentFrame].particle[i][j].p);
        }
    }
#endif
#if 0//enable to show debug information
    gotoxy(20, 20);
#endif
    gotoxy(user.previewOffset[XPOS], user.previewOffset[YPOS] - 2);
    printf("Frame: %d/%d", user.currentFrame, user.maxFrameEdited);

    gotoxy(user.previewOffset[XPOS], user.previewOffset[YPOS] - 3);
    printf("Offest of the current frame: (%d,%d)", frame[user.currentFrame].offset[XPOS], frame[user.currentFrame].offset[YPOS]);

    gotoxy(user.previewOffset[XPOS], user.previewOffset[YPOS] - 4);
    printf("Delay to the next frame: %dms", frame[user.currentFrame].delay);

    gotoxy(0, 1);
    printf("%s", user.commandFeedback);

    gotoxy(0, user.previewOffset[YPOS] + user.previewRulerLength + 1);
    printf("%s", LANG_DESCRIPTION_HELP);

    RESET_CURSOR;
    printf(LANG_PROMPT);
}
///bug: input more than 41 characters(probably has to be a unknown command) when the length of user.input is 128 will cause segfault
///add: str X Y S [-vu][-hu][-vd][-hd](print a string S on (X,Y) vertically or horizontally)
///add: delf
/*
"Help: \"so X Y\" will set the offset of the current frame\n
Help: \"spo X Y\" will set the offset of the preview window\n
Help: \"sr L\" will set the length of the rulers to L\n
Help: \"del X Y\" will delete the character on the (X,Y) coordinate of the current frame\n
Help: \"p X Y C\" will place a character C on the (X,Y) coordinate of the current frame\n
Help: \"nf\" will create a new frame\n
Help: \"sd MS\" will set the delay to the next frame to S miliseconds\n
Help: \"tf F\" will make the program jump to frame F(can't exceed number of frame created)\n
Help: \"fill X1 Y1 X2 Y2 C\" will fill the area specified with the character C\n
Help: \"drange X1 Y1 X2 Y2\" will remove the characters within the area specified
Help: \"output\" will generate the code for your animation\n
Help: \"save FILENAME\" will save the current project with a file name of T.txt\n
Help: \"load FILENAME\" will load the save file with a name of T(without the .txt extension)
Help: \"copy F\" will copy the layout and settings of frame X to this frame\n
Help: \"preview T\" will show the animation in action for T times\n
Help: \"delall\" will delete all characters of the current frame\n"
*/