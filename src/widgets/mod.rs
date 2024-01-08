use gtk::prelude::*;
use gtk::Align;
use mlua::prelude::*;

#[derive(Clone, FromLua)]
pub enum Widget {
    Label(gtk::Label),
    Button(gtk::Button),
}

impl Widget {
    pub fn get_widget(&self) -> gtk::Widget {
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

impl LuaUserData for Widget {
    fn add_fields<'lua, F: mlua::UserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_get("classes", |_, this| this.css_classes());
        fields.add_field_method_set("classes", |_, this, classes| this.set_css_classes(classes));

        fields.add_field_method_get("visible", |_, this| this.visible());
        fields.add_field_method_set("visible", |_, this, visible| this.set_visible(visible));

        fields.add_field_method_get("halign", |_, this| this.halign());
        fields.add_field_method_set("halign", |_, this, halign| this.set_halign(halign));

    }
}
