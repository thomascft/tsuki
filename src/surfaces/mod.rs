use gtk::prelude::*;
use mlua::prelude::*;

#[derive(Clone, FromLua)]

// I'm using an enum similar to the Widget enum so I can eventually add things like a lockscreen
// type 
pub enum Surface {
    Window(gtk::ApplicationWindow)
}

impl Surface {
    fn get_surface(&self) -> gtk::ApplicationWindow {
        match self {
            Self::Window(w) => w.clone()
        }
    }

    // no get child function for now
    fn set_child(&self, child: super::widgets::Widget) -> Result<(), LuaError> {
        let window = self.get_surface();
        let widget = child.get_widget();

        Ok(window.set_child(Some(&widget)))
    }

    fn present(&self) -> Result<(), LuaError> {
        let w = self.get_surface();

        Ok(w.present())
    }
}

impl LuaUserData for Surface {
    fn add_fields<'lua, F: LuaUserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_set("child", |_, this, child| this.set_child(child));
    }
    fn add_methods<'lua, M: LuaUserDataMethods<'lua, Self>>(methods: &mut M) {
        methods.add_method("present", |_, this, ()| this.present());
    }
}
