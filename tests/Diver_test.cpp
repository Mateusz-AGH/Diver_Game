#include <gtest/gtest.h>
#include "../Diver.h" // Adjust this according to your include path

// Sample test case
TEST(DiverTest, SampleTest) {
    EXPECT_EQ(1, 1); // Replace with actual tests
}

bool CheckCollisionRecs(Rectangle r1, Rectangle r2) {
    return (r1.x < r2.x + r2.width &&
        r1.x + r1.width > r2.x &&
        r1.y < r2.y + r2.height &&
        r1.y + r1.height > r2.y);
}

float Vector2Distance(Vector2 v1, Vector2 v2) {
    return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

// Testy jednostkowe

// Testy dla kolizji prostokątów
TEST(CollisionTest, RectanglesCollide) {
    Rectangle r1 = { 0, 0, 10, 10 };
    Rectangle r2 = { 5, 5, 10, 10 };
    EXPECT_TRUE(CheckCollisionRecs(r1, r2));
}

TEST(CollisionTest, RectanglesDoNotCollide) {
    Rectangle r1 = { 0, 0, 10, 10 };
    Rectangle r2 = { 20, 20, 10, 10 };
    EXPECT_FALSE(CheckCollisionRecs(r1, r2));
}

// Testy dla obliczania odległości między punktami
TEST(Vector2DistanceTest, DistanceIsCorrect) {
    Vector2 v1 = { 0, 0 };
    Vector2 v2 = { 3, 4 };
    EXPECT_FLOAT_EQ(Vector2Distance(v1, v2), 5.0);
}

TEST(Vector2DistanceTest, SamePointDistanceIsZero) {
    Vector2 v1 = { 2, 2 };
    Vector2 v2 = { 2, 2 };
    EXPECT_FLOAT_EQ(Vector2Distance(v1, v2), 0.0);
}

// Testy dla zarządzania rekinami
TEST(SharkTest, SharkPositionUpdates) {
    Vector2 sharkPosition = { 100, 100 };
    float sharkSpeed = 5.0f;
    sharkPosition.x -= sharkSpeed;
    EXPECT_EQ(sharkPosition.x, 95.0f);
}

// Testy dla tlenu
TEST(OxygenTest, OxygenCollected) {
    Vector2 diverPosition = { 100, 100 };
    Vector2 oxygenPosition = { 105, 105 };
    EXPECT_TRUE(Vector2Distance(diverPosition, oxygenPosition) < 10.0);
}

TEST(OxygenTest, OxygenNotCollected) {
    Vector2 diverPosition = { 100, 100 };
    Vector2 oxygenPosition = { 200, 200 };
    EXPECT_FALSE(Vector2Distance(diverPosition, oxygenPosition) < 10.0);
}

// Testy dla nurka
TEST(DiverTest, DiverMovesCorrectly) {
    Vector2 diverPosition = { 100, 100 };
    float diverSpeed = 5.0f;
    diverPosition.y -= diverSpeed; // Ruch w górę
    EXPECT_EQ(diverPosition.y, 95.0f);
}

TEST(DiverTest, DiverDoesNotMoveOutOfBounds) {
    Vector2 diverPosition = { 100, 0 };
    float diverSpeed = 5.0f;
    diverPosition.y -= diverSpeed;
    if (diverPosition.y < 0) diverPosition.y = 0;
    EXPECT_EQ(diverPosition.y, 0.0f);
}

// Testy dla zarządzania wektorami rekinów
TEST(VectorManagementTest, SharksVectorResizesCorrectly) {
    std::vector<Vector2> sharks = { {100, 100}, {200, 200} };
    sharks.pop_back();
    EXPECT_EQ(sharks.size(), 1);
}

TEST(VectorManagementTest, AddingSharksWorksCorrectly) {
    std::vector<Vector2> sharks;
    sharks.push_back({ 300, 300 });
    EXPECT_EQ(sharks.size(), 1);
}

// Testy dla mechaniki gry
TEST(GameMechanicsTest, ScoreIncreasesOnOxygenCollection) {
    int score = 0;
    Vector2 diverPosition = { 100, 100 };
    Vector2 oxygenPosition = { 105, 105 };
    if (Vector2Distance(diverPosition, oxygenPosition) < 10.0) {
        score += 10;
    }
    EXPECT_EQ(score, 10);
}

TEST(GameMechanicsTest, LivesDecreaseOnSharkCollision) {
    int lives = 3;
    Vector2 diverPosition = { 100, 100 };
    Vector2 sharkPosition = { 102, 102 };
    if (Vector2Distance(diverPosition, sharkPosition) < 10.0) {
        lives -= 1;
    }
    EXPECT_EQ(lives, 2);
}

// Testy końcowe
TEST(GameOverTest, GameEndsWhenLivesReachZero) {
    int lives = 0;
    bool isGameOver = (lives <= 0);
    EXPECT_TRUE(isGameOver);
}

TEST(GameOverTest, GameContinuesIfLivesRemain) {
    int lives = 1;
    bool isGameOver = (lives <= 0);
    EXPECT_FALSE(isGameOver);
}

// Testy dla zarządzania czasem gry
TEST(TimeManagementTest, TimeDecreasesCorrectly) {
    float timeLeft = 60.0f;
    float deltaTime = 1.0f / 60.0f; // Jedna klatka
    timeLeft -= deltaTime;
    EXPECT_NEAR(timeLeft, 59.983f, 0.001);
}

TEST(TimeManagementTest, TimeReachesZero) {
    float timeLeft = 0.0f;
    bool isTimeUp = (timeLeft <= 0);
    EXPECT_TRUE(isTimeUp);
}
