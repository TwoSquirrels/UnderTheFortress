# Under the Fortress

> [!Caution]
> traP 2025 年 1-Monthon で作り直すことにしたため、このリポジトリはアーカイブしました。  
> 新リポジトリは [traP 内限定でアクセス可能](https://git.trap.jp/1-Monthon_25_15/UnderTheFortress) です。

**© 2024 TwoSquirrels**  
ライセンス: [MIT License](LICENSE)

新感覚 RPG ゲーム「Under the Fortress」のソースコード。現在開発中です。

<!-- TODO: ゲームのダウンロードページ -->

<!-- TODO: スクリーンショット -->

## 開発者・貢献者

- [**りすりす/TwoSquirrels** (GitHub)](https://github.com/TwoSquirrels) ([traP](https://trap.jp/) 部員)

## 対応 OS

- Windows 7 SP1 (64-bit)
- Windows 8.1 (64-bit)
- Windows 10 (64-bit)
- Windows 11

(macOS, Ubuntu, Web 対応予定)

## 使用技術

- 言語: **C++20**
- ゲームフレームワーク: [**Siv3D** v0.6.15](https://github.com/Siv3D/OpenSiv3D/tree/v0.6.15)

## 開発方法

### 前提要件

- [Siv3D の開発者要件](https://siv3d.github.io/ja-jp/download/windows/#11-%E9%96%8B%E7%99%BA%E8%80%85%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0%E8%A6%81%E4%BB%B6)を満たしている。
- [Siv3D v0.6.15 の SDK](https://siv3d.github.io/ja-jp/) がインストールされている。

### 手順

1. Issue に自分がその変更を担当することを意思表示する。
1. この Reposutory の Fork を作業ディレクトリに clone する。
1. `main` branch から、更新したい内容に適した名前の branch を切る。
   ```sh
   git checkout 'branch-name-here'
   ```
1. [Siv3D のコーディングスタイル](https://siv3d.github.io/ja-jp/develop/coding-style/) に従い開発する。commit メッセージもある程度は分かりやすくしましょう。
1. 変更を Fork に push する。
1. 対応する Issue 番号を含めた適切なコメントを書き Pull request を投げる。

### 注意・ヒント

- GitHub アカウントを所持している場合は、Issues や Pull requests などできるだけこのリポジトリ上で議論をしてください。
- 開発途中でも、Draft pull request を投げることで進捗状況が分かりやすくなります。
- Visual Studio に JetBrains による [ReSharper](https://www.jetbrains.com/ja-jp/resharper/) という拡張機能を入れることで、リファクタリングのヒントをくれるなど開発体験を向上できます。GitHub Education なら無料で使えます。
