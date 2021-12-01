Development environment
===

MEIを開発・実行する環境を構築するための資料です。


## 開発環境の構築

MEIMEIはGitで管理されています。

```bash
# Gitプロジェクトのclone
git clone git@github.com:awrznc/MEIMEI.git
cd ./MEIMEI/
```


### Docker

ストレージには4GB以上の空き容量を設けておいてください。

```bash
# Docker image の作成
docker build --no-cache -f ./tools/devenv/Dockerfile -t meimei:latest .

# 実行
docker run --rm -it -v "$(pwd):/opt/meimei" --workdir="/opt/meimei" -p "6080:80" meimei:latest bash -c "bash ./tools/devenv/run.bash"

# 以下のURLにブラウザからアクセス
#   http://localhost:6080/vnc.html

# 実行をキャンセルして、手元のQEMUで以下のように実行してもよい
#   qemu-system-i386 -drive file=./build/meimei.img,format=raw,if=floppy -vnc :1 -audiodev dsound,id=0 -machine pcspk-audiodev=0
```

開発する場合は、実行のコマンドを以下のように変更し、`run.bash`の内容を参考に開発してください。

```bash
docker run --rm -it -v "$(pwd):/opt/meimei" --workdir="/opt/meimei" -p "6080:80" meimei:latest bash
```
