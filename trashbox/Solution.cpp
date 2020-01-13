#include "Common.h"

#include <iostream>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Shape : public IShape {
protected:
    shared_ptr <ITexture> texture_d = nullptr;
    Point position_d = {0, 0};
    Size size_d = {0, 0};

    char GetPixel(int y_i, int x_i) const {
        if (texture_d && y_i < texture_d->GetImage().size() && x_i < texture_d->GetImage()[0].size()) {
            return texture_d->GetImage()[y_i][x_i];
        }
        return '.';
    }

public:
    void SetPosition(Point position_i) override {
        position_d = position_i;
    }

    Point GetPosition() const override {
        return position_d;
    }

    void SetSize(Size size_i) override {
        size_d = size_i;
    }

    Size GetSize() const override {
        return size_d;
    }

    void SetTexture(shared_ptr <ITexture> texture_i) override {
        texture_d = texture_i;
    }

    ITexture* GetTexture() const override {
        return texture_d.get();
    };
};

class Ellipse : public Shape {
public:
    unique_ptr <IShape> Clone() const override {
        auto my_copy = make_unique <Ellipse>();
        my_copy->SetPosition(position_d);
        my_copy->SetSize(size_d);
        my_copy->SetTexture(texture_d);
        return move(my_copy);
    }

    void Draw(Image& image_io) const override {
        int max_height = min(size_d.height, static_cast<int>(image_io.size()) - position_d.y);
        int max_width = max_height == 0 ? 0 : min(size_d.width, static_cast<int>(image_io[0].size()) - position_d.x);
        for (int i = 0; i < max_height; ++i) {
            for (int j = 0; j < max_width; ++j) {
                if (IsPointInEllipse({j, i}, size_d)) {
                    image_io[i + position_d.y][j + position_d.x] = GetPixel(i, j);
                }
            }
        }
    }
};

class Rectangle : public Shape {
public:
    unique_ptr <IShape> Clone() const override {
        auto my_copy = make_unique <Rectangle>();
        my_copy->SetPosition(position_d);
        my_copy->SetSize(size_d);
        my_copy->SetTexture(texture_d);
        return move(my_copy);
    }

    void Draw(Image& image_io) const override {
        int max_height = min(size_d.height, static_cast<int>(image_io.size()) - position_d.y);
        int max_width = max_height == 0 ? 0 : min(size_d.width, static_cast<int>(image_io[0].size()) - position_d.x);
        for (int i = 0; i < max_height; ++i) {
            for (int j = 0; j < max_width; ++j) {
                image_io[i + position_d.y][j + position_d.x] = GetPixel(i, j);
            }
        }
    }
};

// Напишите реализацию функции
unique_ptr <IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Ellipse) {
        return make_unique <Ellipse>();
    } else {
        return make_unique <Rectangle>();
    }
}