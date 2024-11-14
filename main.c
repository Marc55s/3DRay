#include "raylib.h"
#include "raymath.h"

void camera_move(Camera3D *camera) {

    //TraceLog(LOG_INFO, "(%f %f %f) l = %f",camera->target.x,camera->target.y,camera->target.z,Vector3Length(camera->target));

    // Step 1: Calculate forward direction from camera position to target
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));

    // Step 2: Handle movement along forward direction
    if (IsKeyDown(KEY_W)) {
        camera->position.x += forward.x;
        camera->position.z += forward.z;
    } else if (IsKeyDown(KEY_S)) {
        camera->position.x -= forward.x;
        camera->position.z -= forward.z;
    }

    // Step 3: Handle strafing (left-right movement)
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0, 1, 0}));
    if (IsKeyDown(KEY_A)) {
        camera->position.x -= right.x;
        camera->position.z -= right.z;
    } else if (IsKeyDown(KEY_D)) {
        camera->position.x += right.x;
        camera->position.z += right.z;
    }

    // Step 4: Handle rotation based on mouse input
    Vector2 mouseDirection = GetMouseDelta();
    float sensitivity = 0.01f;  // Adjust as needed
    float yaw = -mouseDirection.x * sensitivity;
    float pitch = -mouseDirection.y * sensitivity;

    // Rotate forward vector by yaw and pitch
    Matrix rotation = MatrixRotateXYZ((Vector3){pitch, yaw, 0.0f});
    Vector3 newForward = Vector3Transform(forward, rotation);
    camera->target = Vector3Add(camera->position, newForward);
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1100;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 5.0f, 20.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;   

    float z = 10;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(BLACK);
        BeginMode3D(camera);

        camera_move(&camera);

        DrawCube((Vector3){0,0,0}, 10, 10, 10, WHITE);
        DrawCubeWires((Vector3){0,0,0}, 10, 10, 10, RED);

        EndMode3D();


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
