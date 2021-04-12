//
// Created by Shiki on 2021/4/12.
//

#ifndef NATIVEOPENGLLEARN_DEMO_INCLUDE_IMAGEDEF_H_
#define NATIVEOPENGLLEARN_DEMO_INCLUDE_IMAGEDEF_H_
#include <stdint.h>
#include <malloc.h>
#include <plog/Log.h>

#define IMAGE_FORMAT_RGBA           0x01
#define IMAGE_FORMAT_NV21           0x02
#define IMAGE_FORMAT_NV12           0x03
#define IMAGE_FORMAT_I420           0x04

#define IMAGE_FORMAT_RGBA_EXT       "RGB32"
#define IMAGE_FORMAT_NV21_EXT       "NV21"
#define IMAGE_FORMAT_NV12_EXT       "NV12"
#define IMAGE_FORMAT_I420_EXT       "I420"

typedef struct _tag_NativeImage {
  int width;
  int height;
  int format;
  uint8_t *ppPlane[3];

  _tag_NativeImage() {
    width = 0;
    height = 0;
    format = 0;
    ppPlane[0] = nullptr;
    ppPlane[1] = nullptr;
    ppPlane[2] = nullptr;
  }
} NativeImage;

class NativeImageUtil {
 public:
  static void AllocNativeImage(NativeImage *pImage) {
    if (pImage->height <= 0 || pImage->width <= 0) {
      return;
    }
    switch (pImage->format) {
      case IMAGE_FORMAT_RGBA: {
        pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(pImage->width * pImage->height * 4));
      }
        break;
      case IMAGE_FORMAT_NV12:
      case IMAGE_FORMAT_NV21: {
        pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(pImage->width * pImage->height * 1.5));
        pImage->ppPlane[1] = pImage->ppPlane[0] + pImage->width * pImage->height;
      }
        break;
      case IMAGE_FORMAT_I420: {
        pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(pImage->width * pImage->height * 1.5));
        pImage->ppPlane[1] = pImage->ppPlane[0] + pImage->width * pImage->height;
        pImage->ppPlane[2] = pImage->ppPlane[1] + pImage->width * (pImage->height >> 2);
      }
        break;
      default: {
        PLOGD << "不支持格式:" << pImage->format;
      }
        break;
    }
  }

  static void InitNativeImage(int format, int width, int height, uint8_t *pData, NativeImage *pImage) {
    pImage->format = format;
    pImage->width = width;
    pImage->height = height;
    pImage->ppPlane[0] = pData;

    switch (format) {
      case IMAGE_FORMAT_RGBA: {
        pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(width * height * 4));
        memcpy(pImage->ppPlane[0], pData, width * height * 4);
      }
        break;
      case IMAGE_FORMAT_NV12:
      case IMAGE_FORMAT_NV21: {
        pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(width * height * 1.5));
        memcpy(pImage->ppPlane[0], pData, width * height * 1.5);
        pImage->ppPlane[1] = pImage->ppPlane[0] + width * height;
      }
        break;
      case IMAGE_FORMAT_I420: {
        pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(width * height * 1.5));
        memcpy(pImage->ppPlane[0], pData, width * height * 1.5);
        pImage->ppPlane[1] = pImage->ppPlane[0] + width * height;
        pImage->ppPlane[2] = pImage->ppPlane[1] + width * height / 4;
      }
        break;
      default: break;
    }
  }

  static void FreeNativeImage(NativeImage *pImage) {
    if (!pImage || !(pImage->ppPlane[0])) {
      return;
    }
    free(pImage->ppPlane[0]);
    pImage->ppPlane[0] = nullptr;
    pImage->ppPlane[1] = nullptr;
    pImage->ppPlane[2] = nullptr;
  }

  static void CopyNativeImage(NativeImage *pSrcImage, NativeImage *pDstImage) {
    if (!pSrcImage || !(pSrcImage->ppPlane[0])) {
      return;
    }
    if (pSrcImage->format != pDstImage->format || pSrcImage->width != pDstImage->width
        || pSrcImage->height != pDstImage->height) {
      return;
    }

    if (!(pDstImage->ppPlane[0])) {
      AllocNativeImage(pDstImage);
    }
    switch (pSrcImage->format) {
      case IMAGE_FORMAT_I420:
      case IMAGE_FORMAT_NV21:
      case IMAGE_FORMAT_NV12: {
        memcpy(pDstImage->ppPlane[0], pSrcImage->ppPlane[0], pSrcImage->width * pSrcImage->height * 1.5);
      }
        break;
      case IMAGE_FORMAT_RGBA: {
        memcpy(pDstImage->ppPlane[0], pSrcImage->ppPlane[0], pSrcImage->width * pSrcImage->height * 4);
      }
        break;
      default: {
        PLOGD << "不支持格式:" << pSrcImage->format;
      }
        break;
    }
  }
};
#endif //NATIVEOPENGLLEARN_DEMO_INCLUDE_IMAGEDEF_H_
