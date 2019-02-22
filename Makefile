## BadproG.com
## Makefile

## Variables
NAME        = OGL_Practice.exe
SRC         = window.cpp renderer.cpp IndexBuffer.cpp VertexBuffer.cpp VertexArray.cpp shader.cpp Texture.cpp stb_image.cpp
OBJ         = $(SRC:.cpp=.o)
IMGUI 		= $(wildcard imGui/*.cpp)
IM_OBJ		= $(IMGUI:.cpp=.o)
IM_OBJ2     = $(IMGUI:.cpp=.o)
IM_DEL = $(subst /,\,$(IM_OBJ2));
INCLUDES    = -I"C:\MinGW\mingw64\include"
LIBS        = -L"C:\MinGW\mingw64\lib"
CPPFLAGS    = $(INCLUDES)
LDFLAGS     =  $(LIBS) -lopengl32 -lglu32 -lglew32 -lglfw3dll
CC          = g++

## Rules
$(NAME) : $(OBJ) $(IM_OBJ)
	$(CC) $(OBJ) $(IM_OBJ) -o $(NAME) $(LDFLAGS)
all     : $(NAME)

clean   :
	del /f $(OBJ)
	del /f $(IM_DEL)
fclean  : clean
	del /f $(NAME)
re    : fclean all
	del /f $(OBJ)
	del /f $(IM_DEL)
r    : re
	del /f *~
	del /f *.o