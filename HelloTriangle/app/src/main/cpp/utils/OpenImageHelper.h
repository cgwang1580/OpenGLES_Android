//
// Created by wcg3031 on 2020/4/3.
//

#ifndef HELLOTRIANGLE_OPENIMAGEHELPER_H
#define HELLOTRIANGLE_OPENIMAGEHELPER_H

#include "png.h"

#define CHECK_RET_BREAK(_ret_)	if (0 == _ret_) {MYLOGE ("CHECK_RET_BREAK error"); break;}

class OpenImageHelper
{
public:
	static int LoadPngFromFile (const char* sPath)
	{
		MYLOGD("LoadPngFromFile");
		int ret = 0;
		png_image image {0};
		image.version = PNG_IMAGE_VERSION;
		png_bytep buffer = NULL;
		do
		{
			ret = png_image_begin_read_from_file (&image, sPath);
			MYLOGD("LoadPngFromFile png_image_begin_read_from_file ret = %d", ret);
			CHECK_RET_BREAK (ret)
			image.format = PNG_FORMAT_RGBA;
			buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
			CHECK_MALLOC_BREAK (buffer, "LoadPngFromFile ")
			memset(buffer, 0, PNG_IMAGE_SIZE(image));

			ret = png_image_finish_read(&image, NULL, buffer, 0, NULL);
			MYLOGD("LoadPngFromFile png_image_finish_read ret = %d", ret);
			CHECK_RET_BREAK (ret)

			ret = png_image_write_to_file(&image, "/sdcard/testlibpng.png", 0, buffer, 0, NULL);
			MYLOGD("LoadPngFromFile png_image_write_to_file ret = %d", ret);
			CHECK_RET_BREAK (ret)
		} while(false);

		png_image_free(&image);
		SafeFree(buffer);
		return ret;
	}
};

#endif //HELLOTRIANGLE_OPENIMAGEHELPER_H
