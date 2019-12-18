#
# Makefile
#
TARGET   = $(shell basename $(CURDIR))
#
# Tools
#
CC    = $(SILENT)gcc
CXX   = $(SILENT)g++
LD    = $(SILENT)g++ 
RM    = $(SILENT)rm -f
RMDIR = $(SILENT)rm -rf
MKDIR = $(SILENT)mkdir
ECHO  = @echo

BCM_LIBDIR= /opt/vc/lib

#DEFINE += -DGLFW_INCLUDE_ES2 -D_GLFW_CIRCLE -DIMGUI_IMPL_OPENGL_ES2 -DIMGUI_IMPL_OPENGL_LOADER_GL3W -DSOKOL_GLES2 -DFIPS_RASPBERRYPI -D__circle__ 
COMMON_FLAGS = -DGLFW_INCLUDE_ES2 -DIMGUI_IMPL_OPENGL_ES2 -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM -DSOKOL_GLES2   -D__raspberrypi__
#CIRCLEHOME = ../..
SOURCES = $(shell find . -type f \( -iname "*.c" -o -iname "*.cpp" -o -iname "*.cc" \) -print)
OBJS	= $(shell echo $(SOURCES) | sed -r 's/\.c|\.cpp|.cc/\.o/g')
#OBJS = main.o kernel.o triangle2.o
INCLUDE += -Iimgui -Iinclude -Isokol -Isokol/util -I/opt/vc/include -I/usr/local/include 
INCLUDE += -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
COMMON_FLAGS += -DUSE_EGL -DIS_RPI  $(INCLUDE)
CFLAGS   = -g -w -O3 -ffast-math -fstrict-aliasing -fomit-frame-pointer $(COMMON_FLAGS) 
CPPFLAGS = -g $(COMMON_FLAGS)

LIBS     =  -lz -lbcm_host -lbrcmEGL -lbrcmGLESv2 -lpthread -ludev -lbcm2835 -lasound -lsfml-window -lsfml-system -lsfml-graphics

all: $(OBJS) $(TARGET)

%.o: %.S
	@echo "  AS    $@"
	@$(AS) $(AFLAGS) -c -o $@ $<

%.o: %.c
	@echo "  CC    $@"
	@$(CC) $(CFLAGS) -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(CC) $(CFLAGS) -std=gnu99 -c -o $@ $<

%.o: %.cpp
	@echo "  CPP   $@"
	@$(CXX) $(CPPFLAGS) -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

%.o: %.cc
	@echo "  CPP   $@"
	@$(CXX) $(CPPFLAGS) -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo "  BUILD  $@"
	@$(LD) -o $@ $(OBJS) -L$(BCM_LIBDIR)  $(LIBS) 
	
depend: .depend

.depend: $(SOURCES)
	@rm -f ./.depend
	@$(CC) $(CFLAGS) -MM $^>>./.depend;

include .depend	
	
clean:
	@$(RM) -rf $(OBJS) $(TARGET) $(patsubst %.o,%.d,$(OBJS)) .depend
