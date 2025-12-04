// =============================================================
// Debug Test 3 - UI and Graphics Test
// =============================================================
// Demonstrates UI elements, graphics effects, and various
// rendering techniques and visual tests

#include "debug_test_3.h"
#include "../util/globals.h"
#include "../world/screen_manager.h"
#include "../2d/handler2d.h"
#include "../world/input_manager.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

// Asset manager for fonts and effects
#include "../util/asset_manager.h"

// UI Test state
static bool initialized = false;
static float animationTime = 0.0f;
static int selectedTest = 0;
static const int maxTests = 5;
static bool showGrid = true;
static float colorCycle = 0.0f;

// Fonts for different text styles
static Font titleFont;
static Font uiFont;

// Test names
static const char* testNames[] = {
    "Color Gradients",
    "Shape Animation", 
    "Particle System",
    "Text Rendering",
    "Mouse Interaction"
};

void DebugTest3_Init(void) {
    printf("Initializing Debug Test 3 (UI & Graphics)...\n");
    
    // Switch to debug music
    extern void StartDebugMusic(void);
    StartDebugMusic();
    
    // Generate custom fonts (just use default for now)
    titleFont = GetFontDefault();
    uiFont = GetFontDefault();
    
    animationTime = 0.0f;
    selectedTest = 0;
    showGrid = true;
    colorCycle = 0.0f;
    
    initialized = true;
    printf("✓ Debug Test 3 ready\n");
}

void DebugTest3_Update(float deltaTime) {
    if (!initialized) return;
    
    // Update animation time
    animationTime += deltaTime;
    colorCycle += deltaTime * 2.0f;
    
    // Navigation between tests
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedTest--;
        if (selectedTest < 0) selectedTest = maxTests - 1;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedTest++;
        if (selectedTest >= maxTests) selectedTest = 0;
    }
    
    // Toggle grid
    if (IsKeyPressed(KEY_G)) {
        showGrid = !showGrid;
    }
    
    // Exit back to title screen
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetScreen(SCREEN_STATE_TITLE);
    }
    
    // Switch to first test
    if (IsKeyPressed(KEY_TAB)) {
        SetScreen(SCREEN_STATE_DEBUG1);
    }
}

void DebugTest3_Draw(void) {
    if (!initialized) return;
    
    // Background
    ClearBackground(BLACK);
    
    // Draw grid if enabled
    if (showGrid) {
        for (int x = 0; x < VIRTUAL_SCREEN_WIDTH; x += 40) {
            DrawLine(x, 0, x, VIRTUAL_SCREEN_HEIGHT, (Color){40, 40, 40, 255});
        }
        for (int y = 0; y < VIRTUAL_SCREEN_HEIGHT; y += 40) {
            DrawLine(0, y, VIRTUAL_SCREEN_WIDTH, y, (Color){40, 40, 40, 255});
        }
    }
    
    // Main content area
    Rectangle contentArea = {50, 80, VIRTUAL_SCREEN_WIDTH - 100, VIRTUAL_SCREEN_HEIGHT - 160};
    
    // Draw selected test
    switch (selectedTest) {
        case 0: // Color Gradients
        {
            for (int i = 0; i < contentArea.width; i++) {
                float t = (float)i / contentArea.width;
                Color color1 = {255, 0, 0, 255}; // Red
                Color color2 = {0, 0, 255, 255}; // Blue
                Color blended = {
                    (unsigned char)(color1.r * (1-t) + color2.r * t),
                    (unsigned char)(color1.g * (1-t) + color2.g * t),
                    (unsigned char)(color1.b * (1-t) + color2.b * t),
                    255
                };
                DrawLine(contentArea.x + i, contentArea.y, 
                        contentArea.x + i, contentArea.y + 100, blended);
            }
            
            // Animated color wheel
            for (int i = 0; i < 360; i += 10) {
                float angle = (i + colorCycle * 50.0f) * DEG2RAD;
                float x = contentArea.x + contentArea.width/2 + cosf(angle) * 80;
                float y = contentArea.y + 200 + sinf(angle) * 80;
                
                Color hsvColor = ColorFromHSV(i, 1.0f, 1.0f);
                DrawCircle(x, y, 8, hsvColor);
            }
            break;
        }
        
        case 1: // Shape Animation
        {
            float centerX = contentArea.x + contentArea.width/2;
            float centerY = contentArea.y + contentArea.height/2;
            
            // Rotating squares
            for (int i = 0; i < 5; i++) {
                float rotation = animationTime * 50.0f + i * 72.0f;
                float distance = 100 + sinf(animationTime + i) * 30;
                float x = centerX + cosf(rotation * DEG2RAD) * distance;
                float y = centerY + sinf(rotation * DEG2RAD) * distance;
                
                Vector2 origin = {15, 15};
                Rectangle rect = {x - 15, y - 15, 30, 30};
                
                Color colors[] = {RED, GREEN, BLUE, YELLOW, MAGENTA};
                DrawRectanglePro(rect, origin, rotation, colors[i]);
            }
            
            // Pulsing circle
            float pulseSize = 30 + sinf(animationTime * 3.0f) * 15;
            DrawCircle(centerX, centerY, pulseSize, (Color){255, 255, 255, 128});
            DrawCircleLines(centerX, centerY, pulseSize, WHITE);
            break;
        }
        
        case 2: // Particle System
        {
            // Simple particle effect
            for (int i = 0; i < 50; i++) {
                float t = fmodf(animationTime * 0.5f + i * 0.02f, 1.0f);
                float x = contentArea.x + contentArea.width/2;
                float y = contentArea.y + contentArea.height/2;
                
                float angle = i * 7.2f * DEG2RAD; // 50 particles around circle
                float distance = t * 200.0f;
                float particleX = x + cosf(angle) * distance;
                float particleY = y + sinf(angle) * distance;
                
                float alpha = (1.0f - t) * 255.0f;
                Color particleColor = {255, (unsigned char)(255 * t), 100, (unsigned char)alpha};
                
                float size = 5.0f * (1.0f - t);
                DrawCircle(particleX, particleY, size, particleColor);
            }
            break;
        }
        
        case 3: // Text Rendering
        {
            // Various text styles with custom fonts
            DrawTextEx(titleFont, "LARGE TITLE TEXT", (Vector2){contentArea.x, contentArea.y}, 40, 2, WHITE);
            DrawTextEx(uiFont, "Medium subtitle text", (Vector2){contentArea.x, contentArea.y + 50}, 24, 1, LIGHTGRAY);
            DrawTextEx(uiFont, "Small body text for descriptions", (Vector2){contentArea.x, contentArea.y + 80}, 16, 1, GRAY);
            
            // Animated text with custom font
            float textY = contentArea.y + 120;
            const char* animText = "ANIMATED TEXT";
            int textLength = (int)strlen(animText);
            for (int i = 0; i < textLength; i++) {
                float wave = sinf(animationTime * 5.0f + i * 0.5f) * 10.0f;
                char letter[2] = {animText[i], '\0'};
                Color letterColor = ColorFromHSV(fmodf(animationTime * 100.0f + i * 30.0f, 360.0f), 1.0f, 1.0f);
                DrawTextEx(uiFont, letter, (Vector2){contentArea.x + i * 20, textY + wave}, 20, 1, letterColor);
            }
            
            // Text measurement demo
            const char* measureText = "This text is measured";
            int textWidth = MeasureText(measureText, 16);
            DrawText(measureText, contentArea.x, textY + 60, 16, WHITE);
            DrawRectangleLines(contentArea.x, textY + 60, textWidth, 16, GREEN);
            DrawText(TextFormat("Width: %d pixels", textWidth), contentArea.x, textY + 85, 12, GREEN);
            break;
        }
        
        case 4: // Mouse Interaction
        {
            Vector2 mousePos = GetMousePositionVirtual();
            
            // Draw mouse position
            DrawText(TextFormat("Mouse: (%.0f, %.0f)", mousePos.x, mousePos.y), 
                    contentArea.x, contentArea.y, 16, WHITE);
            
            // Interactive circles
            for (int i = 0; i < 8; i++) {
                float x = contentArea.x + 100 + (i % 4) * 100;
                float y = contentArea.y + 50 + (i / 4) * 100;
                
                float distance = sqrtf(powf(mousePos.x - x, 2) + powf(mousePos.y - y, 2));
                bool isHovered = distance < 30;
                
                Color circleColor = isHovered ? RED : BLUE;
                float size = isHovered ? 35 : 25;
                
                DrawCircle(x, y, size, circleColor);
                DrawCircleLines(x, y, size, WHITE);
                
                if (isHovered) {
                    DrawText("HOVER", x - 20, y - 8, 12, WHITE);
                    
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        DrawText("CLICKED!", x - 25, y + 40, 12, YELLOW);
                    }
                }
            }
            
            // Mouse trail effect
            static Vector2 mouseTrail[20] = {0};
            static int trailIndex = 0;
            
            mouseTrail[trailIndex] = mousePos;
            trailIndex = (trailIndex + 1) % 20;
            
            for (int i = 0; i < 20; i++) {
                float alpha = (float)i / 20.0f * 255.0f;
                DrawCircle(mouseTrail[i].x, mouseTrail[i].y, 3, (Color){255, 255, 255, (unsigned char)alpha});
            }
            break;
        }
    }
    
    // UI Elements with custom fonts
    DrawTextEx(titleFont, "DEBUG TEST 3: UI & GRAPHICS", (Vector2){10, 10}, 20, 1, WHITE);
    DrawTextEx(uiFont, "Up or Down Keys/W or S: Change Test", (Vector2){10, 40}, 14, 1, WHITE);
    DrawTextEx(uiFont, "G: Toggle Grid", (Vector2){10, 55}, 14, 1, WHITE);
    DrawTextEx(uiFont, "TAB: Next Test", (Vector2){10, 70}, 14, 1, WHITE);
    DrawTextEx(uiFont, "ESC: Back to Title", (Vector2){200, 70}, 14, 1, WHITE);
    
    // Test selector
    DrawTextEx(uiFont, "Current Test:", (Vector2){10, VIRTUAL_SCREEN_HEIGHT - 60}, 16, 1, YELLOW);
    DrawTextEx(titleFont, testNames[selectedTest], (Vector2){10, VIRTUAL_SCREEN_HEIGHT - 40}, 20, 1, WHITE);
    
    // Test indicator dots
    for (int i = 0; i < maxTests; i++) {
        Color dotColor = (i == selectedTest) ? YELLOW : DARKGRAY;
        DrawCircle(10 + i * 20, VIRTUAL_SCREEN_HEIGHT - 15, 5, dotColor);
    }
}

void DebugTest3_Unload(void) {
    printf("Unloading Debug Test 3...\n");
    
    // Switch back to main music
    extern void StartMainMusic(void);
    StartMainMusic();
    
    // No custom fonts to unload since we're using default
    
    initialized = false;
}