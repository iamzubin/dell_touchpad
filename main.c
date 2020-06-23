#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <libudev.h>
#include <libinput.h>


#define device_name "DELL07EB:00 06CB:7E7E Touchpad"



static int open_restricted(const char *path, int flags, void *user_data)
{
		int fd = open(path, flags);
		return fd < 0 ? -errno : fd;
}
 
static void close_restricted(int fd, void *user_data)
{
		close(fd);
}
 
const static struct libinput_interface interface = {
		.open_restricted = open_restricted,
		.close_restricted = close_restricted,
};
 
 
static struct libinput_device *get_device(void) {
		struct libinput *li;
		struct libinput_event *event;
		struct udev *udev = udev_new();
		struct libinput_device *device;

		li = libinput_udev_create_context(&interface, NULL, udev);
		libinput_udev_assign_seat(li, "seat0");
		libinput_dispatch(li);
 
		while ((event = libinput_get_event(li)) != NULL) {

				device = libinput_event_get_device(event);
				
				const char *name = libinput_device_get_name(device);
				if (strcmp(device_name,name) == 0)
				return device;

				libinput_event_destroy(event);
				libinput_dispatch(li);
		}
 
		libinput_unref(li);
}

int main(){

	struct libinput_device *device;
	device = get_device();
	if (device != 0){

		const char *name = libinput_device_get_name(device);
		printf("Name : %s \n", name);
	
		int has_gesture = libinput_device_has_capability(device,LIBINPUT_DEVICE_CAP_GESTURE);
		printf("Name : %d \n", has_gesture);

	}

	return 0;
}
