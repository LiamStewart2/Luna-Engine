#include <PyBind/pybind11.h>
#include "Core/LunaWindow.h"  // from engine

namespace py = pybind11;

int get_key(int key) {
    if (LunaWindow::m_FocusedWindow)
        return LunaWindow::m_FocusedWindow->GetKey(key);
    return 0;
}

int get_mouse_button(int button) {
    if (LunaWindow::m_FocusedWindow)
        return LunaWindow::m_FocusedWindow->GetMouseButton(button);
    return 0;
}

std::pair<double, double> get_cursor_position() {
    double x = 0.0, y = 0.0;
    if (LunaWindow::m_FocusedWindow)
        LunaWindow::m_FocusedWindow->GetCursorPosition(&x, &y);
    return { x, y };
}

PYBIND11_MODULE(luna, m) {
    m.doc() = "Luna Engine utility bindings";

    m.def("get_key", &get_key);
    m.def("get_mouse_button", &get_mouse_button);
    m.def("get_cursor_position", &get_cursor_position);
}