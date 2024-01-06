use gtk::Align;
use gtk::prelude::*;
use gtk::glib;
use gtk::Application;

use mlua::prelude::*;
use mlua::{Lua, FromLua, UserData};

use std::fs;
use std::env;
use std::path::PathBuf;

trait WidgetImpl {
    fn get_widget(&self) -> gtk::Widget;

    fn css_classes(&self) -> Result<Vec<String>, LuaError>;
    fn set_css_classes(&self, classes: Vec<String>) -> Result<(), LuaError>;

    fn visible(&self) -> Result<bool, LuaError>;
    fn set_visible(&self, visible: bool) -> Result<(), LuaError>;

    fn halign(&self) -> Result<String, LuaError>;
    fn set_halign(&self, halign: String) -> Result<(), LuaError>;
}

#[derive(Clone, FromLua)]
enum Widget {
    Label(gtk::Label),
    Button(gtk::Button),
}

impl WidgetImpl for Widget {
    fn get_widget(&self) -> gtk::Widget {
        match self {
            Self::Label(w) => w.clone().upcast::<gtk::Widget>(),
            Self::Button(w) => w.clone().upcast::<gtk::Widget>(),
        }
    }

    fn css_classes(&self) -> Result<Vec<String>, LuaError> {
        let w = self.get_widget();

        Ok(w.css_classes()
            .iter()
            .map(|c| c.to_string())
            .collect())
    }
    fn set_css_classes(&self, classes: Vec<String>) -> Result<(), LuaError>{
        let w = self.get_widget();

        Ok(w.set_css_classes(
            &classes.iter()
                .map(|c| c.as_str())
                .collect::<Vec<&str>>()
        ))
    }

    fn visible(&self) -> Result<bool, LuaError> {
        let w = self.get_widget();

        Ok(w.get_visible())
    }
    fn set_visible(&self, visible: bool) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_visible(visible))
    }

    fn halign(&self) -> Result<String, LuaError> {
        let w = self.get_widget();

        match w.halign() {
            Align::End => Ok("end".to_string()),
            Align::Fill => Ok("fill".to_string()),
            Align::Start => Ok("start".to_string()),
            Align::Center => Ok("center".to_string()),
            Align::Baseline => Ok("baseline".to_string()),
            _ => Err("Error matching halign".into_lua_err())
        }
    }
    fn set_halign(&self, halign: String) -> Result<(), LuaError> {
        let w = self.get_widget();

        if "end".to_string() == halign { Ok(w.set_halign(Align::End)) }
        else if "fill".to_string() == halign { Ok(w.set_halign(Align::Fill)) }
        else if "start".to_string() == halign { Ok(w.set_halign(Align::Start)) }
        else if "center".to_string() == halign { Ok(w.set_halign(Align::Center)) }
        else if "baseline".to_string() == halign { Ok(w.set_halign(Align::Baseline)) }
        else { Err(format!("align method: \"{}\" not found", halign).into_lua_err()) }
    }
}

impl UserData for Widget {
    fn add_fields<'lua, F: mlua::UserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_get("classes", |_, this| this.css_classes());
        fields.add_field_method_set("classes", |_, this, classes| this.set_css_classes(classes));

        fields.add_field_method_get("visible", |_, this| this.visible());
        fields.add_field_method_set("visible", |_, this, visible| this.set_visible(visible));

        fields.add_field_method_get("halign", |_, this| this.halign());
        fields.add_field_method_set("halign", |_, this, halign| this.set_halign(halign));

    }
}

fn load_config() -> String {
    let mut fallback_config_path = PathBuf::from(env::var_os("HOME").unwrap_or_default());
    fallback_config_path.push(".config");
    fallback_config_path.push("init");
    fallback_config_path.set_extension("luau");

    let config_path = xdg::BaseDirectories::with_prefix("tsuki").unwrap().place_config_file("init.luau").unwrap_or(fallback_config_path);

    fs::read_to_string(config_path).expect("Failed to read config")
}

fn main() -> glib::ExitCode {
    let app = Application::builder()
        .application_id("com.github.thomascft.tsuki")
        .build();

    app.connect_activate(|app| {
        let lua = Lua::new();

        lua.scope(|scope|{
            let globals = lua.globals();

            let label_new = scope.create_function(|_, label: String| {
                let w = gtk::Label::builder()
                    .label(label)
                    .build();

                Ok(Widget::Label(w))
            })?;
            
            let button_new = scope.create_function(|_, label: String| {
                let w = gtk::Button::builder()
                    .label(label)
                    .build();

                Ok(Widget::Button(w))
            })?;
 
            let test = scope.create_function(|_, w: Widget|{
                gtk::ApplicationWindow::builder()
                    .application(app)
                    .child(&w.get_widget())
                    .build()
                    .present();
                Ok(())
            })?;

            globals.set("label", label_new)?;
            globals.set("button", button_new)?;
            globals.set("test", test)?;

            let config = load_config();
            lua.load(config).exec()
        }).unwrap();
    });

    app.run()
}
