#ifdef ENABLE_GPU

#include "ray2d.h"
#include "keyboard.h"
#include "bus.h"

// ---------- RayBuffer: 离屏 RenderTexture 上的 2D 原语 (双缓冲) ----------

RayBuffer::RayBuffer(int w, int h) : width(w), height(h) {
    target = LoadRenderTexture(w, h);
    front  = LoadRenderTexture(w, h);
    // 初始化为白底, 保证首个 flush 之前的窗口内容确定
    BeginTextureMode(target);
    ClearBackground(WHITE);
    EndTextureMode();
    BeginTextureMode(front);
    ClearBackground(WHITE);
    EndTextureMode();
}

RayBuffer::~RayBuffer() {
    UnloadRenderTexture(target);
    UnloadRenderTexture(front);
}

void RayBuffer::clear_screen() {
    BeginTextureMode(target);
    ClearBackground(WHITE);
    EndTextureMode();
}

void RayBuffer::triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                         int r, int g, int b) {
    BeginTextureMode(target);
    DrawTriangle((Vector2){(float)x0, (float)y0},
                 (Vector2){(float)x1, (float)y1},
                 (Vector2){(float)x2, (float)y2},
                 (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, 255});
    EndTextureMode();
}

void RayBuffer::text(int x0, int y0, int r, int g, int b, int font, const char* str) {
    BeginTextureMode(target);
    DrawText(str, x0, y0, font,
             (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, 255});
    EndTextureMode();
}

void RayBuffer::draw_jpg(const unsigned char* jpg_data, size_t data_length, int x, int y) {
    if (jpg_data == nullptr || data_length == 0) return;
    Image img = LoadImageFromMemory(".jpg", jpg_data, (int)data_length);
    if (img.data == nullptr) return;                 // 解码失败
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    BeginTextureMode(target);
    DrawTexture(tex, x, y, WHITE);
    EndTextureMode();
    UnloadTexture(tex);
}

void RayBuffer::draw_png(const unsigned char* png_data, size_t data_length, int x, int y) {
    if (png_data == nullptr || data_length == 0) return;
    Image img = LoadImageFromMemory(".png", png_data, (int)data_length);
    if (img.data == nullptr) return;                 // 解码失败
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    BeginTextureMode(target);
    DrawTexture(tex, x, y, WHITE);
    EndTextureMode();
    UnloadTexture(tex);
}

void RayBuffer::flush() {
    // 兼容原 flush 语义: 空操作 (帧提交由 present 完成)
}

void RayBuffer::present() {
    // 把 back buffer 整体拷贝到 front buffer (帧完成时调用一次)
    BeginTextureMode(front);
    DrawTextureRec(target.texture,
                   (Rectangle){ 0, 0, (float)width, -(float)height },
                   (Vector2){ 0, 0 }, WHITE);
    EndTextureMode();
}

// ---------- 键盘轮询 ----------

void poll_keyboard(keyboard* kb, Bus* bus) {
    if (kb == nullptr || bus == nullptr) return;

    // 可打印字符 (GetCharPressed 只返回 32~125 的可见 ASCII, 不含 Enter/Tab 等)
    int c;
    while ((c = GetCharPressed()) != 0) {
        kb->push_key((uint8_t)(c & 0xff));
    }

    // 不产生字符的特殊键: 与 FLTK 版本低字节语义一致
    // (Backspace=8, Del=127, Esc=27, Enter='\r'=13, Tab='\t'=9)
    static const struct { int key; uint8_t code; } special[] = {
        { KEY_BACKSPACE, 8   },
        { KEY_DELETE,    127 },
        { KEY_ESCAPE,    27  },
        { KEY_ENTER,     13  },
        { KEY_KP_ENTER,  13  },
        { KEY_TAB,       9   },
    };
    for (size_t i = 0; i < sizeof(special)/sizeof(special[0]); i++) {
        if (IsKeyPressed(special[i].key)) {
            kb->push_key(special[i].code);
        }
    }
}

#endif // ENABLE_GPU
