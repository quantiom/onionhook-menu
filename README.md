# onionhook-menu

Menu for the "onionhook" CS:GO, Garry's Mod and TF2 hack, but it can be used for anything.  
I've stopped working on it in mid/late 2019 because cheating is boring.

## Usage

First, initialize `c_menu(const std::string& bold_name, const std::string& non_bold_name, const Vec2i& pos)`:

```c++
c_menu menu("ONION", "HOOK", { screen_size.x / 2 - (800 / 2), screen_size.y / 2 - (600 / 2) });
```

Then, call `c_menu::update` and `c_menu::draw` in your drawing hook:

```cpp
menu.update();
menu.draw();
```

To disable game input:

```cpp
if (menu.m_open) {
    // disable input
} else {
    // enable input
}
```

Example for adding elements:

```cpp
auto settings_tab = menu.add_element<c_tab>("SETTINGS");
{
    settings_tab->add_element<c_subtab>("TEST")->m_active = true;
    settings_tab->add_element<c_subtab>("OTHER");
}

menu.add_element<c_tab>("PLAYERS");
menu.add_element<c_tab>("MISC");
menu.add_element<c_tab>("VISUALS");
menu.add_element<c_tab>("LEGIT")->add_element<c_checkbox>("Checkbox", test);

auto rage_tab = menu.add_element<c_tab>("RAGE", "test info");
{
    rage_tab->m_active = true;
    rage_tab->add_element<c_subtab>("AIMBOT")->m_active = true;

    auto fakelag_subtab = rage_tab->add_element<c_subtab>("FAKE LAG");
    fakelag_subtab->add_element<c_checkbox>("Test", test);
    fakelag_subtab->add_element<c_checkbox>("Test 2", test);
    fakelag_subtab->add_element<c_checkbox>("Test 3", test);
    fakelag_subtab->add_element<c_group>("GENERAL")->add_element<c_checkbox>("Test 4", test);
    fakelag_subtab->add_element<c_group>("GENERAL2");
    fakelag_subtab->add_element<c_group>("GENERAL3");

    rage_tab->add_element<c_subtab>("ANTI-AIM");
    rage_tab->add_element<c_subtab>("CUSTOM 1");
    rage_tab->add_element<c_subtab>("CUSTOM 2");
    rage_tab->add_element<c_subtab>("CUSTOM 3");
}
```

### Preview

[New Version](https://i.imgur.com/GhJojHv.mp4)  
[Old Version](https://gyazo.com/612099dd564931af86c4174c6abc152f)
