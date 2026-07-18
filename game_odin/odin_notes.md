# Odin Language Notes

## Install

Latest releases: https://github.com/odin-lang/Odin/releases

```
mkdir -p ~/odin
cd ~/Downloads

wget https://github.com/odin-lang/Odin/releases/download/dev-2026-07/odin-linux-amd64-dev-2026-07.tar.gz
tar -xzf odin-linux-amd64-*.tar.gz -C ~/odin --strip-components=1

echo 'export PATH="$HOME/odin:$PATH"' >> ~/.bashrc
source ~/.bashrc

odin version

sudo apt install clang
```


### App TODO

* Audio, music, sound
* joystick input
* touch input
* generate asset binary and definitions


### C++ raylib Set icon

```

void set_window_icon(Window& window, Icon64 const& icon)
{
    rl::Image rimage{};
    rimage.width = icon.width;
    rimage.height = icon.height;
    rimage.data = (void*)icon.pixel_data;
    rimage.mipmaps = 1;

    rl::SetWindowIcon(rimage);
}
```

### Odin Pros & Cons

Pros
* Enum shorthand
* Enumerated arrays

Cons
* Packages - directory mess
* No namespaces - need packages
* No constexpr
* No static_assert
* No ref/const ref

### Wishlist

* Convert C/C++ function to Odin proc