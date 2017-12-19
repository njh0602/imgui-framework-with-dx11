#include "Image.h"

// libpng
//#include "png.h"
//#include "pngconf.h"
//#include "pnglibconf.h"

// zlib
//#include "zlib.h"
//#include "zconf.h"


Image::Image()
{
}


Image::~Image()
{
}


bool Image::initWithPngFile(const std::string & path)
{
	//png_byte header[8];
	//FILE *fp = fopen(path.c_str(), "rb");
	//if (!fp) return false;

	//struct stat statBuf;
	//int descriptor = fileno(fp);
	//if (fstat(descriptor, &statBuf) == -1)
	//{
	//	fclose(fp);
	//	return false;
	//}
	//size_t size = statBuf.st_size;
	//_data = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * size));
	//fread(_data, 1, size, fp);
	//memcpy(header, _data, 8);
	//if (png_sig_cmp(header, 0, 8)) {
	//	free(_data);
	//	fclose(fp);
	//	return false;
	//}

	///* initialize stuff */
	//png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	//if (!png_ptr) {
	//	free(_data);
	//	fclose(fp);
	//	return false;
	//}

	//png_infop info_ptr = png_create_info_struct(png_ptr);
	//if (!info_ptr) {
	//	free(_data);
	//	fclose(fp);
	//	return false;
	//}

	//if (setjmp(png_jmpbuf(png_ptr))) {
	//	free(_data);
	//	fclose(fp);
	//	return false;
	//}
	//
	//fseek(fp, 8, SEEK_SET);
	//png_init_io(png_ptr, fp);
	//png_set_sig_bytes(png_ptr, 8);
	//png_read_info(png_ptr, info_ptr);

	//int width = png_get_image_width(png_ptr, info_ptr);
	//int height = png_get_image_height(png_ptr, info_ptr);

	//png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	//png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	//int number_of_passes = png_set_interlace_handling(png_ptr);
	//png_read_update_info(png_ptr, info_ptr);

	///* read file */
	//if (setjmp(png_jmpbuf(png_ptr))) {
	//	free(_data);
	//	fclose(fp);
	//	return false;
	//}

	//png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	//for (int y = 0; y < height; y++)
	//	row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));

	//png_read_image(png_ptr, row_pointers);

	//fclose(fp);

	//_width = width;
	//_height = height;
	//_fileType = Format::PNG;
	//_filePath = path;
	//_dataLen = size;

	return true;
}
