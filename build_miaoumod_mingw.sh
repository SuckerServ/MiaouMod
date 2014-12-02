wget http://luajit.org/download/LuaJIT-2.0.3.tar.gz
tar -xvf LuaJIT-2.0.3.tar.gz
cd LuaJIT-2.0.3/src
make HOST_CC="gcc -m32" CROSS=i686-w64-mingw32- TARGET_SYS=Windows BUILDMODE=static
sudo cp libluajit.a /usr/i686-w64-mingw32/lib/libluajit-5.1.a

sudo mkdir /usr/i686-w64-mingw32/include/luajit-2.0/
for i in lauxlib.h  luaconf.h  lua.h  lua.hpp  luajit.h  lualib.h; do
	sudo cp ${i} /usr/i686-w64-mingw32/include/luajit-2.0/
done
