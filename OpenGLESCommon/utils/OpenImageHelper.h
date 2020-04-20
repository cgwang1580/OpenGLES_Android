//
// Created by wcg3031 on 2020/4/3.
//

#ifndef HELLOTRIANGLE_OPENIMAGEHELPER_H
#define HELLOTRIANGLE_OPENIMAGEHELPER_H

#include <processor.h>
#include "png.h"
#include "LogAndroid.h"
#include "MyDefineUtils.h"

#define CHECK_PNG_RET_BREAK(_pRet_)	if (0 == *(_pRet_)) {LOGE ("CHECK_PNG_RET_BREAK error"); *(_pRet_) = ERROR_UNKNOWN; break;}

#define MY_FORMAT_RGB		0x100
#define MY_FORMAT_RGBA		0x101

typedef struct __tag_image_info_
{
	int width;
	int height;
	int format;
	int channel[4];
	unsigned char *buffer[4];
}MyImageInfo, *LPMyImageInfo;

class OpenImageHelper
{
public:

	/**
	 * Calculate image buffer length
	 * @param lpMyImageInfo
	 * @return Size of image buffer
	 */
	static int CalMyImageBufferLength (const LPMyImageInfo lpMyImageInfo)
	{
		LOGD("CalMyImageBufferLength");
		if (NULL == lpMyImageInfo || 0 == lpMyImageInfo->width || 0 == lpMyImageInfo->height)
		{
			LOGE("AllocMyImageInfo lpMyImageInfo wrong");
			return 0;
		}
		int lSize = 0;
		switch (lpMyImageInfo->format)
		{
			case MY_FORMAT_RGB:
				lSize = 3 * lpMyImageInfo->channel[0] * lpMyImageInfo->height;
				break;
			case MY_FORMAT_RGBA:
				lSize = 4 * lpMyImageInfo->channel[0] * lpMyImageInfo->height;
				break;
			default:
				break;
		}
		return lSize;
	}

	/**
	 * Alloc MyImageInfo, buffer of image should be null
	 * @param lpMyImageInfo
	 * @return
	 */
	static ERROR_CODE AllocMyImageInfo (LPMyImageInfo lpMyImageInfo)
	{
		LOGD("AllocMyImageInfo");
		if (NULL == lpMyImageInfo || 0 == lpMyImageInfo->width || 0 == lpMyImageInfo->height || NULL != lpMyImageInfo->buffer[0])
		{
			LOGE("AllocMyImageInfo lpMyImageInfo wrong");
			return ERROR_INPUT;
		}

		ERROR_CODE ret = ERROR_OK;
		int lSize = 0;
		lSize = CalMyImageBufferLength(lpMyImageInfo);
		LOGD("AllocMyImageInfo CalMyImageBufferLength lSize = %d", lSize);
		if (0 == lSize)
		{
			LOGE("AllocMyImageInfo lSize = 0");
			return ERROR_INPUT;
		}
		switch (lpMyImageInfo->format)
		{
			case MY_FORMAT_RGB:
			case MY_FORMAT_RGBA:
				lpMyImageInfo->buffer[0] = (unsigned char *) malloc(lSize);
				CHECK_MALLOC_BREAK(lpMyImageInfo->buffer[0], &ret, "AllocMyImageInfo MY_FORMAT_RGB");
				memset(lpMyImageInfo->buffer[0], 0, lSize);
				break;
			default:
				break;
		}
		return ret;
	}

	static ERROR_CODE FreeMyImageInfo (LPMyImageInfo lpMyImageInfo)
	{
		LOGD("FreeMyImageInfo");
		CHECK_NULL_INPUT(lpMyImageInfo)
		SafeFree(lpMyImageInfo->buffer[0]);
		memset(lpMyImageInfo, 0, sizeof(MyImageInfo));
		return ERROR_OK;
	}

	/**
	 *
	 * @param sPath
	 * @param lpMyImageInfo
	 * @return
	 */
	static int LoadPngFromFile (const char* sPath, LPMyImageInfo lpMyImageInfo)
	{
		LOGD("LoadPngFromFile");
		int ret = 0;
		png_image image {0};
		image.version = PNG_IMAGE_VERSION;
		png_bytep buffer = NULL;
		do
		{
			ret = png_image_begin_read_from_file (&image, sPath);
			LOGD("LoadPngFromFile png_image_begin_read_from_file ret = %d", ret);
			CHECK_PNG_RET_BREAK (&ret)
			image.format = PNG_FORMAT_RGBA;
			buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
			CHECK_MALLOC_BREAK (buffer, &ret, "LoadPngFromFile ")
			memset(buffer, 0, PNG_IMAGE_SIZE(image));

			ret = png_image_finish_read(&image, NULL, buffer, 0, NULL);
			LOGD("LoadPngFromFile png_image_finish_read ret = %d", ret);
			CHECK_PNG_RET_BREAK (&ret)

			/*ret = png_image_write_to_file(&image, "/sdcard/testlibpng.png", 0, buffer, 0, NULL);
			LOGD("LoadPngFromFile png_image_write_to_file ret = %d", ret);
			CHECK_PNG_RET_BREAK (ret)*/

			CHECK_NULL_INPUT_BREAK (lpMyImageInfo, &ret, "LoadPngFromFile lpImageInfo")
			lpMyImageInfo->width = static_cast<int>(image.width);
			lpMyImageInfo->height = static_cast<int>(image.height);
			if (PNG_FORMAT_RGBA == image.format) {
				lpMyImageInfo->format = MY_FORMAT_RGBA;
			} else{
				lpMyImageInfo->format = MY_FORMAT_RGB;
			}
			lpMyImageInfo->channel[0] = lpMyImageInfo->width;
			ret = AllocMyImageInfo(lpMyImageInfo);
			if (ERROR_OK != ret)
			{
				LOGE("LoadPngFromFile AllocMyImageInfo ret = %d", ret);
				break;
			}
			memcpy(lpMyImageInfo->buffer[0], buffer, PNG_IMAGE_SIZE(image));

		} while(false);

		png_image_free(&image);
		SafeFree(buffer);
		return ret;
	}

	static ERROR_CODE SaveImageToPng (const LPMyImageInfo lpMyImageInfo, const char* sPath)
	{
		CAL_TIME_COST("SaveImageToPng");
		CHECK_NULL_INPUT(lpMyImageInfo)
		CHECK_NULL_INPUT(lpMyImageInfo->buffer[0])
		CHECK_NULL_INPUT(sPath)
		LOGD("SaveImageToPng sPath = %s", sPath);

		ERROR_CODE ret = ERROR_OK;

		png_image image {0};
		image.version = PNG_IMAGE_VERSION;
		image.width = static_cast<png_uint_32 >(lpMyImageInfo->width);
		image.height = static_cast<png_uint_32 >(lpMyImageInfo->height);
		if (MY_FORMAT_RGBA == lpMyImageInfo->format) {
			image.format = PNG_FORMAT_RGBA;
		} else{
			image.format = PNG_FORMAT_RGB;
		}
		image.colormap_entries = 256;
		png_bytep buffer = lpMyImageInfo->buffer[0];
		int png_ret = png_image_write_to_file(&image, sPath, 0, buffer, 0, NULL);
		LOGD("SaveImageToPng png_image_write_to_file ret = %d", ret);
		if (0 == png_ret)
		{
			LOGE("SaveImageToPng png_image_write_to_file warning_or_error = %d message = %s",
					image.warning_or_error, image.message);
			ret = ERROR_IMAGE;
		}
		return ret;
	}

	static void ExchangeImageCoordinateY (LPMyImageInfo lpMyImageInfo)
	{
		CAL_TIME_COST("ExchangeImageCoordinateY");
		if (NULL == lpMyImageInfo || NULL == lpMyImageInfo->buffer[0])
		{
			LOGE("ExchangeImageCoordinateY ERROR_INPUT");
			return;
		}

		int channelNum = 0;
		switch (lpMyImageInfo->format)
		{
			case MY_FORMAT_RGBA:
				channelNum = 4;
				break;
			case MY_FORMAT_RGB:
				channelNum = 3;
				break;
			default:
				break;
		}

		int pitch = lpMyImageInfo->channel[0];
		int height = lpMyImageInfo->height;
		int lineSize = channelNum * pitch;
		unsigned char *lineBuffer = NULL;
		lineBuffer = (unsigned char *)malloc (lineSize);
		memset(lineBuffer, 0, lineSize);

		int num = height / 2;
		for (int i = 0; i < num; ++i)
		{
			memcpy(lineBuffer, lpMyImageInfo->buffer[0] + i * lineSize, lineSize);
			memcpy(lpMyImageInfo->buffer[0] + i * lineSize, (lpMyImageInfo->buffer[0] + (height - i - 1) * lineSize), lineSize);
			memcpy((lpMyImageInfo->buffer[0] + (height - i - 1) * lineSize), lineBuffer, lineSize);
			memset(lineBuffer, 0, sizeof(lineSize));
		}

		SafeFree(lineBuffer);
		return;
	}
};

#endif //HELLOTRIANGLE_OPENIMAGEHELPER_H
