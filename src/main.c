#include <raylib.h>
#include <raylib-nuklear.h>
#include <vec.h>

#include <stdio.h>
#include <time.h>

vec_str_t logs;
vec_str_t files;

char *currentPath;

void consoleLog(char *text)
{
	vec_push(&logs, text);
}

void updateFilesystem()
{
	int count;
	char **dirFiles = GetDirectoryFiles(currentPath, &count);

	printf("CURRENT PATH: %s\n", currentPath);

	for (int i = 0; i < count; i++)
	{
		vec_push(&files, dirFiles[i]);
	}
}

int main()
{
	vec_init(&logs);
	vec_init(&files);

	currentPath = (char*)GetWorkingDirectory();

	SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1000, 600, "Engine Editor by Vinny Horgan");
	SetTargetFPS(60);

	int width = GetScreenWidth();
	int height = GetScreenHeight();

	struct nk_context *ctx = InitNuklear(0);

	RenderTexture2D viewport = LoadRenderTexture(500, 500);
	Vector2	viewportCursor;
	viewportCursor.x = 0;
	viewportCursor.y = 0;
	Camera2D viewportCam = {0};
	viewportCam.offset = (Vector2){width/2.0f, height/2.0f};
	viewportCam.rotation = 0.0f;
	viewportCam.zoom = 1.0f;

	updateFilesystem();

	while (!WindowShouldClose())
	{
		if (IsWindowResized())
		{
			width = GetScreenWidth();
			height = GetScreenHeight();
		}

		if (IsKeyDown(KEY_W))
			viewportCursor.y += 1;
		if (IsKeyDown(KEY_S))
			viewportCursor.y -= 1;
		if (IsKeyDown(KEY_A))
			viewportCursor.x -= 1;
		if (IsKeyDown(KEY_D))
			viewportCursor.x += 1;

		viewportCam.target = viewportCursor;

		//printf("X: %f, Y: %f\n", viewportCam.target.x, viewportCam.target.y);

		UpdateNuklear(ctx);

		// SCENE PANEL
		if (nk_begin(ctx, "Scene", nk_rect(0, 0, width/4, height/2), NK_WINDOW_TITLE|NK_WINDOW_BORDER))
		{

		}
		nk_end(ctx);

		// INSPECTOR PANEL
		if (nk_begin(ctx, "Inspector", nk_rect(width/4*3, 0, width/4, height), NK_WINDOW_TITLE|NK_WINDOW_BORDER))
		{

		}
		nk_end(ctx);

		// VIEWPORT PANEL
		if (nk_begin(ctx, "Viewport", nk_rect(width/4, 0, width/2, height/4*3), NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
		{
			nk_layout_row_static(ctx, height/4*3 - 40, width/2 - 10, 1);
			nk_image(ctx, TextureToNuklear(viewport.texture));
		}
		nk_end(ctx);

		// CONSOLE PANEL
		if (nk_begin(ctx, "Console", nk_rect(width/4, height/4*3, width/2, height/4), NK_WINDOW_TITLE|NK_WINDOW_BORDER))
		{
			nk_layout_row_dynamic(ctx, 12, 1);

			int i; char *val;
			vec_foreach(&logs, val, i)
			{
				nk_label(ctx, val, NK_TEXT_LEFT);
			}
		}
		nk_end(ctx);

		// FILESYSTEM PANEL
		if (nk_begin(ctx, "Filesystem", nk_rect(0, height/2, width/4, height/2), NK_WINDOW_TITLE|NK_WINDOW_BORDER))
		{
			nk_layout_row_dynamic(ctx, 12, 1);

			int i; char *val;
			vec_foreach(&files, val, i)
			{
				if (nk_button_label(ctx, val)) {}
			}
		}
		nk_end(ctx);

		BeginDrawing();

			ClearBackground(GRAY);

			DrawNuklear(ctx);

			BeginTextureMode(viewport);

				ClearBackground(SKYBLUE);

				BeginMode2D(viewportCam);

					DrawRectangle(0, 0, 25, 25, GREEN);

				EndMode2D();

			EndTextureMode();

		EndDrawing();
	}

	UnloadNuklear(ctx);

	CloseWindow();

	return 0;
}