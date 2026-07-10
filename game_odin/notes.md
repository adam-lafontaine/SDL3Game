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
```