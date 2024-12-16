#include "raylib.h"
#include <math.h>
#include <stdio.h>

typedef struct Vertex {
  float x, y, z;
} Vertex;

typedef struct Triangle {
  int v1;
  int v2;
  int v3;
} Triangle;

// Rotate a point around the X-axis
Vertex RotateX(Vertex point, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);

  float yNew = point.y * c - point.z * s;
  float zNew = point.y * s + point.z * c;

  return (Vertex){point.x, yNew, zNew};
}

// Rotate a point around the Y-axis
Vertex RotateY(Vertex point, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);

  float xNew = point.x * c + point.z * s;
  float zNew = -point.x * s + point.z * c;

  return (Vertex){xNew, point.y, zNew};
}

// Rotation!!
Vertex RotateZ(Vertex point, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);

  float xNew = point.x * c - point.y * s;
  float yNew = point.x * s + point.y * c;

  return (Vertex){xNew, yNew, point.z};
}
// draws 3d point to 2d
Vector2 Project3D(Vertex point, float screenWidth, float screenHeight) {
  float fov = 300.0f;      // Field of view
  float distance = 500.0f; // Distance from the camera
  float scale = fov / (fov + point.z + distance);

  float screenX = screenWidth / 2.0f + point.x * scale;
  float screenY = screenHeight / 2.0f - point.y * scale;

  return (Vector2){screenX, screenY};
}

int main() {
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 600;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Rotating Cube");
  SetTargetFPS(60);

  // Define cube vertices in 3D space
  Vertex vertices[] = {
      {-100, -100, -100}, // Back bottom-left
      {100, -100, -100},  // Back bottom-right
      {100, 100, -100},   // Back top-right
      {-100, 100, -100},  // Back top-left
      {-100, -100, 100},  // Front bottom-left
      {100, -100, 100},   // Front bottom-right
      {100, 100, 100},    // Front top-right
      {-100, 100, 100}    // Front top-left
  };

  // Define cube triangles (each face has 2 triangles)
  Triangle triangles[] = {// Back face
                          {0, 1, 2},
                          {0, 2, 3},

                          // Front face
                          {4, 5, 6},
                          {4, 6, 7},

                          // Left face
                          {0, 3, 7},
                          {0, 7, 4},

                          // Right face
                          {1, 5, 6},
                          {1, 6, 2},

                          // Top face
                          {3, 2, 6},
                          {3, 6, 7},

                          // Bottom face
                          {0, 1, 5},
                          {0, 5, 4}};

  float angleX = 0.0f; // Rotation angle around the X-axis
  float angleY = 0.0f; // Rotation angle around the Y-axis
  float angleZ = 0.0f; // Rotation angle around the Z-axis

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Rotate vertices and project them to 2D space
    Vertex rotatedVertices[8];
    for (int i = 0; i < 8; i++) {
      Vertex rotated = vertices[i];

      // Apply rotations around X, Y, and Z axes
      rotated = RotateX(rotated, angleX);
      // uncomment to rotate in Y-axis
      /* rotated = RotateY(rotated, angleY); */
      rotated = RotateZ(rotated, angleZ);

      rotatedVertices[i] = rotated;
    }

    // Draw all triangles
    for (int i = 0; i < sizeof(triangles) / sizeof(Triangle); i++) {
      Vector2 v1 = Project3D(rotatedVertices[triangles[i].v1], SCREEN_WIDTH,
                             SCREEN_HEIGHT);
      Vector2 v2 = Project3D(rotatedVertices[triangles[i].v2], SCREEN_WIDTH,
                             SCREEN_HEIGHT);
      Vector2 v3 = Project3D(rotatedVertices[triangles[i].v3], SCREEN_WIDTH,
                             SCREEN_HEIGHT);

      // Use a different color for each face
      Color color = (i / 2) % 2 == 0 ? DARKGRAY : LIGHTGRAY;

      DrawTriangle(v1, v2, v3, color);
      DrawLineV(v1, v2, RAYWHITE);
      DrawLineV(v2, v3, RAYWHITE);
      DrawLineV(v3, v1, RAYWHITE);
    }

    EndDrawing();

    // Increment rotation angles for continuous rotation
    angleX += 0.01f;  // Rotation speed around X-axis
    angleY += 0.02f;  // Rotation speed around Y-axis
    angleZ += 0.015f; // Rotation speed around Z-axis
  }

  CloseWindow();
  return 0;
}
