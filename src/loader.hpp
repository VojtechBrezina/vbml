#pragma once

#include <vector>
#include <string>
#include <compare>
#include <istream>

/// A simple container for the lines of source code. It allows indexing both
/// lines (by size_t) and characters (by SourcePos).
class SourceFile {
    std::vector<std::string> lines;

    public:
        /// The line:column pair with comparisons and the ability to calculate
        /// disatances and perform movements in the context of a given
        /// SourceFile.
        /// @note Note to self: circular dependency sucks.
        struct Pos {
            size_t line = 0;
            size_t column = 0;

            /// According to the documentation, the default one should be fine,
            /// but this will probably be safer.
            std::strong_ordering operator <=>(const Pos &other) const;

            /// <=> should generate this, but whatever, default works.
            bool operator ==(const Pos &other) const = default;

            /// A "pure function" if you would like to obtain a moved copy. This
            /// is fine to do for a 16 byte struct I think.
            Pos moved(ssize_t chars, const SourceFile &code) const;

            /// Could be used to calculate token lengths while iterating.
            ssize_t distanceTo(
                const Pos &other, const SourceFile &code
            ) const;

            /// Only really useful for internal implementation of moved and
            /// maybe iterating through SourceFiles.
            void moveForward(size_t chars, const SourceFile &code);

            /// Just for moved as it is probably useless, but there is no reason
            /// to make it private, especcially on a struct.
            void moveBack(size_t chars, const SourceFile &code);
        };

        /// Read all of the given stream and make an instance.
        SourceFile(std::istream &input);

        SourceFile() = delete;

        std::string &operator [](size_t line){ return lines[line]; }
        const std::string &operator [](size_t line) const {
            return lines[line];
        }

        char &operator [](const Pos &pos){
            return lines[pos.line][pos.column];
        }

        const char &operator [](const Pos &pos) const {
            return lines[pos.line][pos.column];
        }

        size_t lineCount() const { return lines.size(); }
};

using SourcePos = SourceFile::Pos;
