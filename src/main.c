#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <libudev.h>
#include <libinput.h>


#define device_name "DELL07EB:00 06CB:7E7E Touchpad"
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


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


int main(void) {
	struct libinput *li;
	struct libinput_event *event;
	struct udev *udev = udev_new();
	struct udev_device *udev_dev = NULL;
	struct libinput_device *device;

	char buffer[EVENT_BUF_LEN];

	li = libinput_udev_create_context(&interface, NULL, udev);
	libinput_udev_assign_seat(li, "seat0");
	libinput_dispatch(li);
	int fd = libinput_get_fd(li);

	while ((event = libinput_get_event(li)) != NULL) {

		device = libinput_event_get_device(event);

		const char *name = libinput_device_get_name(device);
		if (strcmp(device_name,name) == 0)
		{
			udev_dev = libinput_device_get_udev_device(device);
			const char *path = udev_device_get_devnode(udev_dev);
			printf("%s", path);
			int file;
			file = inotify_add_watch(fd, path,IN_ACCESS );
			int length = read(fd,buffer ,EVENT_BUF_LEN );
			printf("%d", length);
			while ((length = read(3, buffer, BUFSIZ)) > 0)
				write(1, buffer, length);
		}
		libinput_event_destroy(event);
		libinput_dispatch(li);
	}

	libinput_unref(li);
	return 1;
}
