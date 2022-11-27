#include <am.h>
#include <klib.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  printf("init: \n");
  int i;
  int w = inw(VGACTL_ADDR);      // TODO: get the correct width
  int h = inw(VGACTL_ADDR + 2);  // TODO: get the correct height
  printf("w: %d, h: %d\n", w, h);
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T){.present = true,
                           .has_accel = false,
                           .width = inw(VGACTL_ADDR + 2),
                           .height = inw(VGACTL_ADDR),
                           .vmemsz = 0};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  const int H = inw(VGACTL_ADDR);
  const int W = inw(VGACTL_ADDR + 2);
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  // printf("H: %d, W: %d,", H, W);
  // printf("x: %d, y: %d, w:%d, h: %d\n", x, y, w, h);
  uint32_t *pixels = ctl->pixels;
  int cp_bytes = sizeof(uint32_t) * (w < W - x ? w : W - x);
  for (int j = 0; j < h && y + j < H; j++) {
    memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
    // printf("pixels: ");
    // for (int df = 0; df < w; df++) {
      // printf("0x%x ", *(pixels + df));
    // }
    // printf("\n");
    pixels += w;
  }
  if (ctl->sync) {
    // printf("sync: %u\n", ctl->sync);
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) { status->ready = true; }
