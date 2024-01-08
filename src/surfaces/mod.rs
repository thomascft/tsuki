use std::collections::HashMap;

use gtk::prelude::*;
use mlua::prelude::*;

use gtk4_layer_shell::{Edge, Layer, LayerShell};

#[derive(Clone, FromLua)]

// I'm using an enum similar to the Widget enum so I can eventually add things like a lockscreen
// type
pub enum Surface {
    Window(gtk::ApplicationWindow),
}

impl Surface {
    fn get_surface(&self) -> gtk::ApplicationWindow {
        match self {
            Self::Window(w) => w.clone(),
        }
    }

    // no get child function for now
    fn set_child(&self, child: super::widgets::Widget) -> Result<(), LuaError> {
        let window = self.get_surface();
        let widget = child.get_widget();

        Ok(window.set_child(Some(&widget)))
    }
    
    fn layer(&self) -> Result<String, LuaError> {
        let w = self.get_surface();

        match w.layer() {
            Layer::Background => Ok("background".to_string()),
            Layer::Bottom => Ok("bottom".to_string()),
            Layer::Top => Ok("top".to_string()),
            Layer::Overlay => Ok("overlay".to_string()),
            _ => Err("Error getting window layer".into_lua_err())

        }
    }

    fn set_layer(&self, layer: String) -> Result<(), LuaError> {
        let w = self.get_surface();

        if layer == "background".to_string() { Ok(w.set_layer(Layer::Background)) }
        else if layer == "bottom".to_string() { Ok(w.set_layer(Layer::Bottom)) }
        else if layer == "top".to_string() { Ok(w.set_layer(Layer::Top)) }
        else if layer == "overlay".to_string() { Ok(w.set_layer(Layer::Overlay)) }
        else { Err(format!("{layer} is not a valid layer").into_lua_err()) }
    }

    fn present(&self) -> Result<(), LuaError> {
        let w = self.get_surface();

        Ok(w.present())
    }
}

impl LuaUserData for Surface {
    fn add_fields<'lua, F: LuaUserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_set("child", |_, this, child| this.set_child(child));

        fields.add_field_method_get("layer", |_, this| this.layer());
        fields.add_field_method_set("layer", |_, this, layer| this.set_layer(layer));
    }
    fn add_methods<'lua, M: LuaUserDataMethods<'lua, Self>>(methods: &mut M) {
        methods.add_method("present", |_, this, ()| this.present());
    }
}
