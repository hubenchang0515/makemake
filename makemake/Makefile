makemake: config.o generator.o main.o target.o utils.o
	g++ -o $@ $^ -lstdc++fs
config.o: config.cpp config.h target.h utils.h \
 include/rapidjson/document.h include/rapidjson/reader.h \
 include/rapidjson/allocators.h include/rapidjson/rapidjson.h \
 include/rapidjson/internal/meta.h \
 include/rapidjson/internal/../rapidjson.h include/rapidjson/stream.h \
 include/rapidjson/encodings.h include/rapidjson/encodedstream.h \
 include/rapidjson/memorystream.h include/rapidjson/internal/clzll.h \
 include/rapidjson/internal/stack.h \
 include/rapidjson/internal/../allocators.h \
 include/rapidjson/internal/swap.h include/rapidjson/internal/strtod.h \
 include/rapidjson/internal/ieee754.h \
 include/rapidjson/internal/biginteger.h \
 include/rapidjson/internal/diyfp.h include/rapidjson/internal/clzll.h \
 include/rapidjson/internal/pow10.h include/rapidjson/error/error.h \
 include/rapidjson/error/../rapidjson.h \
 include/rapidjson/internal/strfunc.h \
 include/rapidjson/internal/../stream.h \
 include/rapidjson/internal/../rapidjson.h include/rapidjson/writer.h \
 include/rapidjson/internal/dtoa.h include/rapidjson/internal/itoa.h \
 include/rapidjson/internal/itoa.h include/rapidjson/stringbuffer.h \
 include/rapidjson/stringbuffer.h
	g++ -c config.cpp -O3 -W -Wall -Wextra -std=c++17 -Iinclude
generator.o: generator.cpp generator.h utils.h
	g++ -c generator.cpp -O3 -W -Wall -Wextra -std=c++17 -Iinclude
main.o: main.cpp utils.h target.h config.h
	g++ -c main.cpp -O3 -W -Wall -Wextra -std=c++17 -Iinclude
target.o: target.cpp target.h utils.h
	g++ -c target.cpp -O3 -W -Wall -Wextra -std=c++17 -Iinclude
utils.o: utils.cpp utils.h
	g++ -c utils.cpp -O3 -W -Wall -Wextra -std=c++17 -Iinclude

