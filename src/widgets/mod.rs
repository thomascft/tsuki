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

        Ok(w.css_classes().iter().map(|c| c.to_string()).collect())
    }
    fn set_css_classes(&self, classes: Vec<String>) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_css_classes(&classes.iter().map(|c| c.as_str()).collect::<Vec<&str>>()))
    }

    fn valign(&self) -> Result<String, LuaError> {
        let w = self.get_widget();

        match w.valign() {
            Align::End => Ok("end".to_string()),
            Align::Fill => Ok("fill".to_string()),
            Align::Start => Ok("start".to_string()),
            Align::Center => Ok("center".to_string()),
            Align::Baseline => Ok("baseline".to_string()),
            _ => Err("Error matching valign".into_lua_err()),
        }
    }
    fn set_valign(&self, valign: String) -> Result<(), LuaError> {
        let w = self.get_widget();

        if "end".to_string() == valign {
            Ok(w.set_valign(Align::End))
        } else if "fill".to_string() == valign {
            Ok(w.set_valign(Align::Fill))
        } else if "start".to_string() == valign {
            Ok(w.set_valign(Align::Start))
        } else if "center".to_string() == valign {
            Ok(w.set_valign(Align::Center))
        } else if "baseline".to_string() == valign {
            Ok(w.set_valign(Align::Baseline))
        } else {
            Err(format!("align method: \"{}\" not found", valign).into_lua_err())
        }
    }

    fn halign(&self) -> Result<String, LuaError> {
        let w = self.get_widget();

        match w.halign() {
            Align::End => Ok("end".to_string()),
            Align::Fill => Ok("fill".to_string()),
            Align::Start => Ok("start".to_string()),
            Align::Center => Ok("center".to_string()),
            Align::Baseline => Ok("baseline".to_string()),
            _ => Err("Error matching halign".into_lua_err()),
        }
    }
    fn set_halign(&self, halign: String) -> Result<(), LuaError> {
        let w = self.get_widget();

        if "end".to_string() == halign {
            Ok(w.set_halign(Align::End))
        } else if "fill".to_string() == halign {
            Ok(w.set_halign(Align::Fill))
        } else if "start".to_string() == halign {
            Ok(w.set_halign(Align::Start))
        } else if "center".to_string() == halign {
            Ok(w.set_halign(Align::Center))
        } else if "baseline".to_string() == halign {
            Ok(w.set_halign(Align::Baseline))
        } else {
            Err(format!("align method: \"{}\" not found", halign).into_lua_err())
        }
    }

    fn vexpand(&self) -> Result<bool, LuaError> {
        let w = self.get_widget();

        Ok(w.vexpands())
    }
    fn set_vexpand(&self, vexpand: bool) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_vexpand(vexpand))
    }

    fn hexpand(&self) -> Result<bool, LuaError> {
        let w = self.get_widget();

        Ok(w.hexpands())
    }
    fn set_hexpand(&self, hexpand: bool) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_hexpand(hexpand))
    }

    fn width(&self) -> Result<i32, LuaError> {
        let w = self.get_widget();

        Ok(w.width_request())
    }
    fn set_width(&self, width: i32) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_width_request(width))
    }

    fn height(&self) -> Result<i32, LuaError> {
        let w = self.get_widget();

        Ok(w.height_request())
    }
    fn set_height(&self, height: i32) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_height_request(height))
    }

    fn active(&self) -> Result<bool, LuaError> {
        let w = self.get_widget();

        Ok(w.is_sensitive())
    }
    fn set_active(&self, active: bool) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_sensitive(active))
    }
    fn tooltip(&self) -> Result<String, LuaError> {
        let w = self.get_widget();

        Ok(w.tooltip_text()
            .expect("couldn't get tooltip text")
            .to_string())
    }
    fn set_tooltip(&self, tooltip: String) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_tooltip_text(Some(&tooltip)))
    }

    fn visible(&self) -> Result<bool, LuaError> {
        let w = self.get_widget();

        Ok(w.get_visible())
    }
    fn set_visible(&self, visible: bool) -> Result<(), LuaError> {
        let w = self.get_widget();

        Ok(w.set_visible(visible))
    }
}

impl LuaUserData for Widget {
    fn add_fields<'lua, F: mlua::UserDataFields<'lua, Self>>(fields: &mut F) {
        fields.add_field_method_get("classes", |_, this| this.css_classes());
        fields.add_field_method_set("classes", |_, this, classes| this.set_css_classes(classes));

        fields.add_field_method_get("valign", |_, this| this.valign());
        fields.add_field_method_set("valign", |_, this, valign| this.set_valign(valign));

        fields.add_field_method_get("halign", |_, this| this.halign());
        fields.add_field_method_set("halign", |_, this, halign| this.set_halign(halign));

        fields.add_field_method_get("vexpand", |_, this| this.vexpand());
        fields.add_field_method_set("vexpand", |_, this, vexpand| this.set_vexpand(vexpand));

        fields.add_field_method_get("hexpand", |_, this| this.hexpand());
        fields.add_field_method_set("hexpand", |_, this, hexpand| this.set_hexpand(hexpand));

        fields.add_field_method_get("width", |_, this| this.width());
        fields.add_field_method_set("width", |_, this, width| this.set_width(width));

        fields.add_field_method_get("height", |_, this| this.height());
        fields.add_field_method_set("height", |_, this, height| this.set_height(height));

        fields.add_field_method_get("active", |_, this| this.active());
        fields.add_field_method_set("active", |_, this, active| this.set_active(active));

        fields.add_field_method_get("tooltip", |_, this| this.tooltip());
        fields.add_field_method_set("tooltip", |_, this, tooltip| this.set_tooltip(tooltip));

        fields.add_field_method_get("visible", |_, this| this.visible());
        fields.add_field_method_set("visible", |_, this, visible| this.set_visible(visible));
    }
}
