#include "svg.h"
#include <memory>

namespace svg {

using namespace std;
using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "/>"sv;
}

// ---------- Polyline ------------------

Polyline& Polyline::AddPoint(Point point) {
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<polyline points=\""sv;
    bool isFirst = true;
    for (const auto& point : points_) {
        if (!isFirst) {
            out << " "sv;
        }
        isFirst = false;
        out << point.x << ","sv << point.y;
    }
    out << "\" />"sv;
}

// ---------- Text ------------------
Text& Text::SetPosition(Point pos) {
    pos_ = pos;
    return *this;
}

Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

Text& Text::SetFontSize(uint32_t size) {
    size_ = size;
    return *this;
}

Text& Text::SetFontFamily(string font_family) {
    font_family_ = font_family;
    return *this;
}

Text& Text::SetFontWeight(string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

Text& Text::SetData(string data) {
    data_ = data;
    return *this;
}

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text"sv
        << " x=\""sv << pos_.x << "\""sv
        << " y=\""sv << pos_.y << "\""sv
        << " dx=\""sv << offset_.x << "\""sv
        << " dy=\""sv << offset_.y << "\""sv
        << " font-size=\""sv << size_ << "\""sv;
        if (!font_family_.empty()) {
            out << " font-family=\""sv << font_family_ << "\""sv;
        }
        if (!font_weight_.empty()) {
            out << " font-weight=\""sv << font_weight_ << "\""sv;
        }
        out << ">"sv;

        for (const auto& c : data_) {
            switch (c) {
                case '"':
                    out << "&quot;";
                    break;
                case '\'':
                    out << "&apos;";
                    break;
                case '<':
                    out << "&lt;";
                    break;
                case '>':
                    out << "&gt;";
                    break;
                case '&':
                    out << "&amp;";
                    break;
                default:
                    out << c;
                    break;
            }
        }

        out << "</text>"sv;
}

// ---------- Document ------------------

void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    objects_.push_back(move(obj));
}

void Document::Render(std::ostream& out) const {
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << endl;

    RenderContext ctx(std::cout, 2, 2);
    for (const auto& obj : objects_) {
        obj->Render(ctx);
    }

    out << "</svg>"sv;
}

}  // namespace svg
