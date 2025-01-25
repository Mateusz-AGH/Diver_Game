#include "Diver.h"
#include "raylib.h"
#include <vector>


using namespace std;
typedef enum GameScreen {TITLE, GAMEPLAY, ENDING } GameScreen;

// Class Shark
class Shark {
public:
    Vector2 sharkposition;
    Rectangle frameRec;
    Rectangle body;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    float speed;    // Shark speed in X

    // Constructor
    Shark(Vector2 startPos, float animSpeed, float sharkSpeed, Texture2D tex)
        :   sharkposition(startPos),
            framesCounter(0),
            framesSpeed((int)animSpeed),
            speed(sharkSpeed),
            currentFrame(0)
    {
        frameRec = { 0.0f, 0.0f, (float)tex.width / 8, (float)tex.height };

        body = { sharkposition.x, sharkposition.y, frameRec.width, frameRec.height };
    }


    // Updating Sharks 
    void Update(Texture2D tex, float deltaTime)
    {
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
        }

        if (currentFrame >= 8) currentFrame = 0;
        frameRec.x = (float)currentFrame * frameRec.width;

        sharkposition.x -= speed * deltaTime;

        body.x = sharkposition.x;
        body.y = sharkposition.y;
    }

    // Drawing Shark
    void Draw(Texture2D tex) const {
        DrawTexturePro(tex, frameRec, { sharkposition.x, sharkposition.y, 200, 200 }, { 100, 100 }, 0.0f, WHITE);
    }

    // Checking if shark is off screen
    bool IsOffScreen()
    {
        return (body.x + frameRec.width - 20 < 0);
    }
};
// Class Oxygen
class OxygenBottle {
public:
    Rectangle body; // for now oxygen is rectangle
    float speed;    // Shark speed in X

    // Constructor
    OxygenBottle(float x, float y, float width, float height, float speedX)
    {
        body = { x, y, width, height };
        speed = speedX;
    }

    // Updating OxygenBottle speed
    void Update()
    {
        body.x -= speed;
    }

    // Drawing OxygenBottle
    void Draw(Texture2D tex) const
    {
        DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, {body.x, body.y, 100, 100}, {50, 50}, 0.0f, WHITE);
    }

    // Checking if shark is off screen
    bool IsOffScreen()
    {
        return (body.x + body.width - 20 < 0);
    }
};

int main(void)
{
    // Initialization
    //------------------------------------------------------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Diver");
    SetWindowState(FLAG_WINDOW_RESIZABLE); // Resiziable Window
    GameScreen currentScreen = TITLE;


    // Loading Textures
    Texture2D texheart = LoadTexture("resources/heart_32x32.png");
    Texture2D texBackground = LoadTexture("resources/Background.png");
    Texture2D texDiver = LoadTexture("resources/Diver_sheet.png");
    Texture2D texShark = LoadTexture("resources/SharkAll.png");
    Texture2D texOxygenBottle = LoadTexture("resources/OxygenBottleO2.png");
    Texture2D texTitleScreen = LoadTexture("resources/titlescreen.png");
    Texture2D texEndingScreen = LoadTexture("resources/endingscreen.png");


    // Score
    int Score = 0;
    int bonus = 0;
    bool ShowScoreMessage = false;

    // Adjusting Collision Signal
    float CollisionmessageTimer = 0.0f; // Timer for the Collsion message
    float PickUpmessageTimer = 0.0f; // Timer for the PickUp message
    bool showCollsionMessage = false;  // Flag to control Shark message visibility
    bool showPickUpMessage = false; // Flag to control Oxygen message visivilty

    // Shark
    vector<Shark> sharks; // vector for Sharks
    float sharkSpawnTimer = 0; // Shark Timer
    const float sharkSpawnInterval = 2.0f; //sec before new shark


    // OxygenBottle
    vector<OxygenBottle> oxygenBottles; // Vector for Oxygen Bottles
    float oxygenSpawnTimer = 0;         // Timer for spawning Oxygen Bottle
    const float oxygenSpawnInterval = 3.0f; // 5 seconds for Oxygen Bottle spawn

    // Health and Oxygen
    int health = 5;         // Player has 5 health
    float oxygen = 100.0f;  // PLayer starts with full O2

    // Invincibility
    bool isInvincible = false;  // Invcincible Flag 
    float invincibleTimer = 0.0f; // Invincible Timer
    const float invincibleDuration = 1.5f; // Invincible Duration

    // Diver
    Vector2 Diver_Position = { (float)screenWidth / 2, (float)screenHeight / 2 };
    Rectangle frameRec = { 0.0f, 128.0f, (float)texDiver.width/12, (float)texDiver.height/6 };
    Color diversColor = WHITE;

    // Collision
    bool collisionShark = false;
    bool collisionOxygenBottle = false;

    // Frame Counting
    int currentFrame = 0;
    int framesCounter = 0;
    int DiverframesSpeed = 15;

    // Audio
    InitAudioDevice();                  // Initialize audio device
    Music music = LoadMusicStream("resources/mini1111.xm");
    music.looping = true;   //music looping 
    float pitch = 0.5f; // "speed" of the song
    PlayMusicStream(music); // play music
    bool pause = false; // Pausing the music
    bool musicstart = true; // Flag for the music change



    // Game Speed
    float elapsedTime = 0.0f; // Time of playing in sec
    int lastIncreaseTime = 0;
    int IncreaseInterval = 10.0f;

    //starting message
    bool sendDiveMessage = 1;

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)

    // Main game loop
    //----------------------------------------------------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        switch (currentScreen)
            {
            case TITLE:
            {
                // Score
                Score = 0;
                bonus = 0;
                ShowScoreMessage = false;

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = GAMEPLAY;
                }
                pause = true;
                if (pause) {
                    StopMusicStream(music);
                    musicstart = true;
                }

            } break;
            case GAMEPLAY:
            {

                elapsedTime += GetFrameTime(); // Time of last Frame

                // Score
                Score = elapsedTime + bonus;

                // Dive message
                if (elapsedTime <= 2) {
                    sendDiveMessage = 1;
                }
                if (elapsedTime > 2) {
                    sendDiveMessage = 0;
                }

                // Music Logic
                UpdateMusicStream(music);      // Update music buffer with new stream data
                pause = false;
                if (!pause && musicstart) {
                    SetMusicPitch(music, pitch);
                    PlayMusicStream(music);
                    musicstart = false;

                }

                // Speeding Up the game
                if ((int)elapsedTime >= lastIncreaseTime + IncreaseInterval) {
                    if (pitch <= 1.5) {
                        pitch += 0.05f;
                        SetMusicPitch(music, pitch);
                    }

                    if (DiverframesSpeed <= 22) {
                        DiverframesSpeed += 1;
                    }
                    lastIncreaseTime = (int)elapsedTime;
               }

                // Frames counting used in animation
                framesCounter++;
                if (framesCounter >= (60 / DiverframesSpeed))
                {
                    framesCounter = 0;
                    currentFrame++;

                    if (currentFrame >= 12) currentFrame = 0;

                    frameRec.x = (float)currentFrame * (float)texDiver.width / 12;
                }

                // Invincibility logic
                if (isInvincible)
                {
                    diversColor = RED;  // Red Tint after being damaged
                    invincibleTimer += GetFrameTime(); 
                    if (invincibleTimer >= invincibleDuration)
                    {
                        isInvincible = false; // Turn off being Invincible after 1.5sec
                        invincibleTimer = 0.0f; // Reset the Timer
                        diversColor = WHITE;
                    }
                }

                // Change Players position based on arrow keys
                if (IsKeyDown(KEY_RIGHT)) Diver_Position.x += 5.0f;
                if (IsKeyDown(KEY_LEFT)) Diver_Position.x -= 5.0f;
                if (IsKeyDown(KEY_UP)) Diver_Position.y -= 5.0f;
                if (IsKeyDown(KEY_DOWN)) Diver_Position.y += 5.0f;

                oxygen -= 5.0f * GetFrameTime(); // Oxygen being lost every sec
                if (oxygen <= 0.0f || health <= 0)
                {
                    //printf("Game Over! \n");
                    currentScreen = ENDING; // Change to the Ending Screen
                }

                // Spawning the Sharks + speeding them Up
                float sharkSpeedMultiplier = 1.0f + elapsedTime / 50.0f;
                const float minSharkSpawnInterval = 0.3f;
                float currentSharkSpawnInterval = fmax(sharkSpawnInterval - elapsedTime / 25.0f, minSharkSpawnInterval);

                printf("currentSharkSpawnInterval: %f \n", currentSharkSpawnInterval);

                sharkSpawnTimer += GetFrameTime();
                if (sharkSpawnTimer >= currentSharkSpawnInterval)
                {
                    float randomY = GetRandomValue(0, screenHeight - 100);
                    sharks.push_back(Shark({ (float)screenWidth, randomY }, 5.0f * sharkSpeedMultiplier, 200.0f * sharkSpeedMultiplier, texShark)); // Adding Shark
                    sharkSpawnTimer = 0; // Timer Reset
                }


                // Sharks updating
                for (size_t i = 0; i < sharks.size();)
                {
                    sharks[i].Update(texShark, GetFrameTime());

                    // Deleting Sharks that are off screen
                    if (sharks[i].IsOffScreen())
                    {
                        sharks.erase(sharks.begin() + i);
                        continue;
                    }

                    




                    //hecking for collision && Adjusting Collision Message
                    collisionShark = (CheckCollisionCircleRec(Diver_Position, 50, sharks[i].body));
                    if (collisionShark && !isInvincible)
                    {
                        health--;   // Lose hp
                        isInvincible = true;    // Enable being invincible
                        invincibleTimer = 0.0f; // Reset the Timer
                        showCollsionMessage = true;
                        CollisionmessageTimer = GetTime(); // Set Timer to current time
                    }

                    // Check if messege time has ended
                    if (showCollsionMessage && (GetTime() - CollisionmessageTimer > 1.5f)) // 1.5s for warning
                    {
                        showCollsionMessage = false; // Stop displaying message
                    }

                    i++; // Going to the next element if not deleted
                    
                }

                // Spawning the OxygenBottles
                oxygenSpawnTimer += GetFrameTime();
                if (oxygenSpawnTimer >= oxygenSpawnInterval)
                {
                    bool validPosition = true;
                    float randomY = GetRandomValue(0, screenHeight - 50); // Random Y

                    for (const Shark& shark : sharks)
                    {
                        Rectangle potentialBottle = { screenWidth, randomY, 50, 50 }; // Size and Poistion of OxygenBottle
                        if (CheckCollisionRecs(potentialBottle, shark.body))
                        {
                            validPosition = false;
                            break;
                        }
                    }
                    // If Valid position Sprawn OxygenBottle
                    if (validPosition)
                    {
                        oxygenBottles.push_back(OxygenBottle(screenWidth, randomY, 50, 50, 3.0f * sharkSpeedMultiplier)); // Speed = 2.0f
                        oxygenSpawnTimer = 0; // Timer reset
                        printf("Oxygen speed: %f \n", 3.0f * sharkSpeedMultiplier);
                    }

                }
                
                // Updating OxygenBottles
                for (size_t i = 0; i < oxygenBottles.size();)
                {
                    oxygenBottles[i].Update();

                    // Erease Bottle if off screen
                    if (oxygenBottles[i].IsOffScreen())
                    {
                        oxygenBottles.erase(oxygenBottles.begin() + i);
                        continue;
                    }

                    collisionOxygenBottle = (CheckCollisionCircleRec(Diver_Position, 30, oxygenBottles[i].body));
                    if (collisionOxygenBottle)
                    {
                        oxygenBottles.erase(oxygenBottles.begin() + i); // Delete Bottle if PickedUp
                        oxygen = fmin(oxygen + 20.0f, 100.0f);          // Add O2 (maks. 100)
                        printf("Oxygen: %f\n", oxygen);                 // Debug tlenu
                        showPickUpMessage = true;
                        PickUpmessageTimer = GetTime(); // Set Timer to current time
                        bonus += 10;
                        continue; // Przejdź do następnej iteracji
                    }

                    // Check if messege time has ended
                    if (showPickUpMessage && (GetTime() - PickUpmessageTimer > 1.5f)) // 0.1s
                    {
                        showPickUpMessage = false; // Stop displaying message
                    }

                    i++; //going to the next element only if not deleted

                }
                


                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
            {

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = TITLE;
                }
                pause = true;
                if (pause) {
                    StopMusicStream(music);
                    musicstart = true;
                }

                //RESET ALL AFTER ENDING GAME
                // -----------------------------------------------------------------------------------------
                // Health and Oxygen
                health = 5;         // Player has 5 health
                oxygen = 100.0f;  // PLayer starts with full O2

                // Adjusting Collision Signal
                CollisionmessageTimer = 0.0f; // Timer for the Collsion message
                PickUpmessageTimer = 0.0f; // Timer for the PickUp message
                showCollsionMessage = false;  // Flag to control Shark message visibility
                showPickUpMessage = false; // Flag to control Oxygen message visivilty

                //Shark
                sharkSpawnTimer = 0; // Shark Timer
                sharks.clear();

                //Oxygen
                oxygenSpawnTimer = 0;         // Timer for spawning Oxygen Bottle
                oxygenBottles.clear();

                // Invincibility
                isInvincible = false;  // Invcincible Flag 
                invincibleTimer = 0.0f; // Invincible Timer

                // Diver
                Diver_Position = { (float)screenWidth / 2, (float)screenHeight / 2 };
                frameRec = { 0.0f, 128.0f, (float)texDiver.width / 12, (float)texDiver.height / 6 };
                diversColor = WHITE;

                // Collision
                collisionShark = false;
                collisionOxygenBottle = false;

                // Frame Counting
                currentFrame = 0;
                framesCounter = 0;          // Useful to count frames
                DiverframesSpeed = 15;

                // Music
                float pitch = 0.5f; // "speed" of the song
                PlayMusicStream(music); // play music
                bool pause = false; // Pausing the music
                bool musicstart = true; // Flag for the music change

                //Time
                elapsedTime = 0;

                // Score Displaying
                ShowScoreMessage = true;




            } break;
        default: break;
        }

        // Draw
        //-----------------------------------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
            case TITLE:
            {
                DrawTexture(texTitleScreen, 0, 0, WHITE);
            } break;
            case GAMEPLAY:
            {
                // Starting Message
                DrawTexture(texBackground, 0, 0, WHITE);
                if (sendDiveMessage) {
                    DrawText("DIVE!", 710, 335, 250, DARKBLUE);
                }
                
                // Diver Animation
                DrawTexturePro(texDiver, frameRec, { Diver_Position.x, Diver_Position.y, 150, 150}, {75,75}, 0,  diversColor);  // Draw part of the texture

                //DrawTexture(texDiver, 15, 40, WHITE);
                //DrawRectangleLines(15, 40, texDiver.width, texDiver.height, LIME);
                //DrawRectangleLines(15 + (int)frameRec.x, 40 + (int)frameRec.y, (int)frameRec.width, (int)frameRec.height, RED);

                for (const Shark& shark : sharks)
                    shark.Draw(texShark);

                for (const OxygenBottle& bottle : oxygenBottles)
                    bottle.Draw(texOxygenBottle);
                
                // Messages
                if (showCollsionMessage)
                {
                    //Draw Collision message
                    DrawText("BEWARE OF SHARKS!", 500, 10, 50, RED);
                }

                if (showPickUpMessage)
                {
                    //Draw PickUp message
                    DrawText("BREATHE!", 500, 60, 50, RAYWHITE);
                }

                // Health hearts
                for (int i = 0; i < health; i++)
                {
                    DrawTexture(texheart, 30 + i * 40, 30, WHITE);
                }

                // Oxygen bar
                DrawRectangle(20, 70, (int)(oxygen * 2), 20, SKYBLUE); // O2 Bar (100 * 2 = 200 px)
                DrawRectangleLines(20, 70, 200, 20, BLACK); // Obrys (maks. 200 px)

                // Score
                DrawText(TextFormat("SCORE: %05i", Score), 20, 110, 40, WHITE);

                //
            } break;
            case ENDING:
            {
                DrawTexture(texEndingScreen, 0, 0, WHITE);
                DrawText(TextFormat("SCORE: %05i", Score), 730, 750, 50, WHITE);
            } break;
            default: break;
        }

        EndDrawing();
    }

    UnloadTexture(texheart); // Jeśli już nie potrzebujesz serc
    UnloadTexture(texBackground);
    UnloadTexture(texDiver);
    UnloadTexture(texShark);
    UnloadTexture(texOxygenBottle);
    UnloadTexture(texTitleScreen);
    UnloadTexture(texBackground);
    UnloadMusicStream(music);          // Unload music stream buffers from RAM
    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)


    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}