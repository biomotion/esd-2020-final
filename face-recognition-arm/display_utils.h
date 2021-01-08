#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/hal/interface.h>
#include <fstream>

typedef struct
{
  uint32_t bytes_per_pixel; // depth of framebuffer
  uint32_t xres_virtual;   // how many pixel in a row in virtual screen
  uint32_t yres_virtual;
} framebuffer_info;

framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
  framebuffer_info fb_info;             // Used to return the required attrs.
  struct fb_var_screeninfo screen_info; // Used to get attributes of the device from OS kernel.

  int fd;

  if ((fd = open(framebuffer_device_path, O_RDWR)) < 0)
  {
    printf("DEVICE CAN'T BE OPENED");
    exit(-1);
  }

  ioctl(fd, FBIOGET_VSCREENINFO, &screen_info);

  fb_info.xres_virtual = screen_info.xres_virtual;     // 8
  fb_info.bytes_per_pixel = screen_info.bits_per_pixel / 8; // 16
  fb_info.yres_virtual = screen_info.yres_virtual;
  printf("screen_res = %dx%d, bits_per_pixel = %d\n", fb_info.xres_virtual, fb_info.yres_virtual, fb_info.bytes_per_pixel);
  return fb_info;
}

int draw_on_screen(cv::InputArray in_frame, std::ofstream &fb, framebuffer_info &fb_info, cv::Rect region)
{
  int y;
  cv::Mat frame = in_frame.getMat();
  cv::Size2i img_size = in_frame.size();

  // Check size
  if (((region.x + region.width) > fb_info.xres_virtual) ||
      ((region.y + region.height) > fb_info.yres_virtual))
  {
    fprintf(stderr, "Error: input region exceed frame size\n");
    return -1;
  }

  if (img_size.width == region.width &&
      img_size.height == region.height)
  {
    cv::cvtColor(frame, frame, cv::COLOR_BGR2BGR565);
    for (y = 0; y < region.height; y++)
    {
      fb.seekp(((region.y + y) * fb_info.xres_virtual + region.x) * fb_info.bytes_per_pixel);
      fb.write(frame.ptr<char>(y, 0), region.width * fb_info.bytes_per_pixel);
    }
  }
  else
  {
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(region.width, region.height));
    cv::cvtColor(resized, resized, cv::COLOR_BGR2BGR565);
    for (y = 0; y < region.height; y++)
    {
      fb.seekp(((region.y + y) * fb_info.xres_virtual + region.x) * fb_info.bytes_per_pixel);
      fb.write(resized.ptr<char>(y, 0), region.width * fb_info.bytes_per_pixel);
    }
  }
  return 0;
}