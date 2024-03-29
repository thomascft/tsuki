use gtk::glib;
use gtk::prelude::*;
use gtk::Application;
use gtk4_layer_shell::{Edge, Layer, LayerShell};

use mlua::prelude::*;
use std::path::PathBuf;

mod widgets;
use widgets::Widget;

mod surfaces;
use surfaces::Surface;

fn get_config_path() -> PathBuf {
    let xdg_dirs = xdg::BaseDirectories::with_prefix("tsuki").expect("Failed to init xdg_dirs");

    xdg_dirs.get_config_home()
}

fn main() -> glib::ExitCode {
    let app = Application::builder()
        .application_id("com.github.thomascft.tsuki")
        .build();

    app.connect_activate(|app| {
        let lua = Lua::new();

        let globals = lua.globals();
        let mut config_path = get_config_path();

        // Add ~/.config/tsuki/ to package.path
        globals
            .get::<_, LuaTable>("package")
            .expect("Failed to get package table")
            .set(
                "path",
                format!("{0}?.luau;{0}?/init.luau", config_path.to_str().unwrap()),
            )
            .expect("Failed to set package.path");

        lua.scope(|scope| {
            let label_new = scope.create_function(|_, label: String| {
                let w = gtk::Label::builder().label(label).build();

                Ok(Widget::Label(w))
            })?;

            let button_new = scope.create_function(|_, label: String| {
                let w = gtk::Button::builder().label(label).build();

                Ok(Widget::Button(w))
            })?;

            let window_new = scope.create_function(|_, ()| {
                let w = gtk::ApplicationWindow::builder().application(app).build();

                w.init_layer_shell();
                w.set_layer(Layer::Overlay);
                w.auto_exclusive_zone_enable();
                
                let anchors = [
                    (Edge::Left, true),
                    (Edge::Right, true),
                    (Edge::Top, true),
                    (Edge::Bottom, false),
                ];

                for (anchor, state) in anchors {
                    w.set_anchor(anchor, state);
                }

                Ok(Surface::Window(w))
            })?;

            globals.set("label", label_new)?;
            globals.set("button", button_new)?;
            globals.set("window", window_new)?;

            config_path.push("init.luau");
            lua.load(config_path).exec()
        })
        .unwrap_or_else(|e| {
            println!("{e}");
            app.quit()
        });
    });

    app.run()
}
