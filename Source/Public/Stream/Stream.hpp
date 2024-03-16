/*
Copyright (c) 2024 M00NLI6H7 (M00NLI6H7@protonmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Assertion.hpp"
#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"

#include <array>
#include <vector>

namespace LibKore
{
    template<typename T>
    constexpr bool isStandardInputStream()
    {
        return std::is_same_v<T, std::istream> ||
               std::is_same_v<T, std::istringstream> ||
               std::is_same_v<T, std::ifstream> ||
               std::is_same_v<T, std::wistream> ||
               std::is_same_v<T, std::wistringstream> ||
               std::is_same_v<T, std::wifstream>;
    }

    template<typename T>
    constexpr bool isStandardOutputStream()
    {
        return std::is_same_v<T, std::ostream> ||
               std::is_same_v<T, std::ostringstream> ||
               std::is_same_v<T, std::ofstream> ||
               std::is_same_v<T, std::wostream> ||
               std::is_same_v<T, std::wostringstream> ||
               std::is_same_v<T, std::wofstream>;
    }

    template<typename T>
    constexpr bool isStandardInputOutputStream()
    {
        return std::is_same_v<T, std::stringstream> ||
               std::is_same_v<T, std::fstream> ||
               std::is_same_v<T, std::wstringstream> ||
               std::is_same_v<T, std::wfstream>;
    }

    enum class ByteOrder : uint8_t
    {
        NORMAL,
        REVERSE
    };

    template<class T>
    concept StandardInputStream = isStandardInputStream<T>() || isStandardInputOutputStream<T>();

    template<class T>
    concept StandardOutputStream = isStandardOutputStream<T>() || isStandardInputOutputStream<T>();

    template<class T>
    concept StandardInputOutputStream = isStandardInputOutputStream<T>();

    template<typename TStream, ByteOrder ReadMode = ByteOrder::NORMAL, ByteOrder WriteMode = ByteOrder::NORMAL>
    class StandardStreamWrapper;

    template<typename T, typename TStream, ByteOrder ReadMode = ByteOrder::NORMAL, ByteOrder WriteMode = ByteOrder::NORMAL>
    concept Readable = requires(StandardStreamWrapper<TStream, ReadMode, WriteMode>& stream) {
        {
            T::read(stream)
        } -> std::same_as<T>;
    };

    template<typename T, typename TStream, ByteOrder ReadMode = ByteOrder::NORMAL, ByteOrder WriteMode = ByteOrder::NORMAL>
    concept Writale = requires(T& object, StandardStreamWrapper<TStream, ReadMode, WriteMode>& stream) {
        {
            object.write(stream)
        } -> std::convertible_to<void>;
    };

    template<typename TStream, ByteOrder ReadMode, ByteOrder WriteMode>
    class StandardStreamWrapper final
    {
      public:
        using CharType = TStream::char_type;

        template<class... Args>
        explicit StandardStreamWrapper(Args&&... args)
            : m_stream(args...)
        {
            if constexpr(isStandardInputStream<TStream>() || isStandardInputOutputStream<TStream>())
            {
                seekRead(0, std::ios::end);
                m_size = readPosition();
                seekRead(0, std::ios::beg);
            }
        }

        ~StandardStreamWrapper()
        {
            m_stream.close();
        }

        [[nodiscard]] inline bool isOpen()
        {
            return m_stream.is_open();
        }

        inline void close()
        {
            return m_stream.close();
        }

        [[nodiscard]] inline bool eof()
        {
            return m_stream.eof();
        }

        [[nodiscard]] inline std::size_t readPosition()
            requires StandardInputStream<TStream> || StandardInputOutputStream<TStream>
        {
            return m_stream.tellg();
        }

        [[nodiscard]] inline std::size_t writePosition() const
            requires StandardOutputStream<TStream> || StandardInputOutputStream<TStream>
        {
            return m_stream.tellp();
        }

        [[nodiscard]] inline std::size_t size() const
            requires StandardInputStream<TStream> || StandardInputOutputStream<TStream>
        {
            return m_size;
        }

        inline void seekRead(std::size_t position, std::ios::seekdir seekType = std::ios::seekdir::cur)
            requires StandardInputStream<TStream> || StandardInputOutputStream<TStream>
        {
            m_stream.seekg(position, seekType);
        }

        inline void seekWrite(std::size_t position)
            requires StandardOutputStream<TStream> || StandardInputOutputStream<TStream>
        {
            m_stream.seekp(position);
        }

        template<typename T>
        [[nodiscard]] inline T read()
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            return readInternal<T>();
        }

        template<typename T>
        [[nodiscard]] inline T read()
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && Readable<T, TStream, ReadMode, WriteMode>
        {
            return T::read(*this);
        }

        template<typename T, std::size_t size>
        [[nodiscard]] inline std::array<T, size> readSome()
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            std::array<T, size> array{};
            for(std::size_t index = 0; index < size; index++)
            {
                array[index] = read<T>();
            }

            return array;
        }

        template<typename T, std::size_t size>
        [[nodiscard]] inline std::array<T, size> readSome()
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && Readable<T, TStream, ReadMode, WriteMode>
        {
            std::array<T, size> array{};
            for(std::size_t index = 0; index < size; index++)
            {
                array[index] = T::read(*this);
            }

            return array;
        }

        template<typename T>
        [[nodiscard]] inline std::vector<T> readSome(std::size_t count)
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            std::vector<T> vector{};
            vector.reserve(count);
            for(std::size_t index = 0; index < count; index++)
            {
                vector.emplace_back(read<T>());
            }

            return vector;
        }

        template<typename T>
        [[nodiscard]] inline std::vector<T> readSome(std::size_t count)
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && Readable<T, TStream, ReadMode, WriteMode>
        {
            std::vector<T> vector{};
            vector.reserve(count);
            for(std::size_t index = 0; index < count; index++)
            {
                vector.emplace_back(T::read(*this));
            }

            return vector;
        }

        template<typename T>
        inline void write(T object)
            requires(StandardOutputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            writeInternal(&object, sizeof(object));
        }

        template<typename T>
        inline void write()
            requires(StandardOutputStream<TStream> || StandardInputOutputStream<TStream>) && Writale<T, TStream, ReadMode, WriteMode>
        {
            return T::read(*this);
        }

      private:
        template<typename T>
        inline T readInternal()
        {
            LIBKORE_VERIFY_THROW(isOpen(), LibKore::Exceptions::RuntimeException, "Stream is not open!");

            const std::size_t readSize = sizeof(T);
            const std::size_t readPos = readPosition();

            LIBKORE_VERIFY_THROW(!eof(), LibKore::Exceptions::RuntimeException, fmt::format("stream EOF at {:#08x}! Read size was {}", readPosition(), readSize));
            LIBKORE_VERIFY_THROW(readPos + readSize <= m_size, LibKore::Exceptions::RuntimeException, fmt::format("Attempt to read {} bytes while {} is available", readSize, m_size - readPos));

            T read{};
            m_stream.read((char*)&read, readSize);

            if constexpr(ReadMode == ByteOrder::REVERSE)
            {
                read = swapEndian(read);
            }

            return read;
        }

        inline void writeInternal(const CharType* data, std::size_t size)
            requires StandardOutputStream<TStream> || StandardInputOutputStream<TStream>
        {
            m_stream.write(data, size);
        }

        template<typename T>
        static T swapEndian(const T& data)
        {
            static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

            union ByteObjectRepresentation
            {
                T object;
                uint8_t bytes[sizeof(T)];
            };

            ByteObjectRepresentation source{};
            ByteObjectRepresentation dst{};

            source.object = data;

            for(size_t k = 0; k < sizeof(T); k++)
            {
                dst.bytes[k] = source.bytes[sizeof(T) - k - 1];
            }

            return dst.object;
        }

      private:
        TStream m_stream;
        std::size_t m_size;
    };

    using StandardInputStreamWrapper = StandardStreamWrapper<std::istream>;
    using StandardOutputStreamWrapper = StandardStreamWrapper<std::ofstream>;

    using StandardWideInputStreamWrapper = StandardStreamWrapper<std::wistream>;
    using StandardWideOutputStreamWrapper = StandardStreamWrapper<std::wostream>;

    using StandardFileInputStreamWrapper = StandardStreamWrapper<std::ifstream>;
    using StandardFileOutputStreamWrapper = StandardStreamWrapper<std::ofstream>;
    using StandardFileStreamWrapper = StandardStreamWrapper<std::fstream>;
    using StandardWideFileInputStreamWrapper = StandardStreamWrapper<std::wifstream>;
    using StandardWideFileOutputStreamWrapper = StandardStreamWrapper<std::wofstream>;
} // namespace LibKore