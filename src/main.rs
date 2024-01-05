use gtk::prelude::*;
use gtk::glib;
use gtk::Application;

use mlua::{Lua, FromLua, UserData};

#[derive(Clone, FromLua)]
enum Widget {
    Label(gtk::Label),
    Button(gtk::Button)
}
impl Widget {
    fn get(&self) -> gtk::Widget {
        match self {
            Widget::Label(label) => label.clone().upcast::<gtk::Widget>(),
            Widget::Button(button) => button.clone().upcast::<gtk::Widget>()
        }
    }
}
impl UserData for Widget {
    fn add_fields<'lua, F: mlua::UserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_get("label", |_, this|{
            let text = match this {
                Widget::Label(label) => label.label().to_string(),
                Widget::Button(button) => button.label().unwrap().to_string()
            };

            Ok(text)
        })
    }
}

#[derive(Clone, FromLua)]
enum Window {
    Window(gtk::ApplicationWindow),
}
impl UserData for Window {
    fn add_fields<'lua, F: mlua::UserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_set("child", |_, this, widget: Widget|{
            match this {
                Window::Window(window) => window.set_child(Some(&widget.get())),
            }
            Ok(())
        });
    }
    fn add_methods<'lua, M: mlua::UserDataMethods<'lua, Self>>(methods: &mut M) {
        methods.add_method("present", |_, this, ()| {
            match this {
                Window::Window(window) => window.present(),
            }
            Ok(())
        });
    }
} 

fn main() -> glib::ExitCode {
    let app = Application::builder()
        .application_id("com.github.thomascft.tsuki")
        .build();

    app.connect_activate(|app| {
        let lua = Lua::new();

        lua.scope(|scope|{
            let window_new = scope.create_function(|_, ()|{
                let window = gtk::ApplicationWindow::builder()
                    .application(app)
                    .build();
                Ok(Window::Window(window))
            })?;
            let label_new = scope.create_function(|_, text: String|{
                let label = gtk::Label::builder()
                    .label(text)
                    .build();

                Ok(Widget::Label(label))
            })?; 
            let button_new = scope.create_function(|_, text: String|{
                let button = gtk::Button::builder()
                    .label(text)
                    .build();

                Ok(Widget::Button(button))
            })?; 

            let globals = lua.globals();
            globals.set("window", window_new)?;
            globals.set("label", label_new)?;
            globals.set("button", button_new)?;

            lua.load(r#"
                print("Hello World!")

                local window = window()
                local label = label("test")

                window.child = label
                window:present()

            "#).exec()
        }).unwrap();
    });

    app.run()
}
