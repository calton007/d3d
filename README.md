# D3D11 教学示例合集

这个项目把旧 Direct3D 9 示例整理并迁移成可运行的 Direct3D 11 Win32 教学示例。

## 环境要求

- Windows
- Visual Studio Build Tools，包含 C++ 工具链
- Windows 10/11 SDK

项目使用 Direct3D 11、DirectXMath 和运行时编译的 HLSL。  
不需要安装 June 2010 DirectX SDK，也不依赖第三方库。

## 构建

打开 Visual Studio Developer Command Prompt，然后运行：

```bat
msbuild d3d.sln /p:Configuration=Debug /p:Platform=x64
```

如果普通终端里找不到 `msbuild`，可以这样运行：

```bat
cmd /c "call ""C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"" -arch=x64 -host_arch=x64 && msbuild d3d.sln /p:Configuration=Debug /p:Platform=x64"
```

## 运行

每个示例都会生成一个独立 exe：

```bat
bin\x64\Debug\<示例名>\<示例名>.exe
```

常用示例：

```bat
bin\x64\Debug\triangle\triangle.exe
bin\x64\Debug\arrowhead\arrowhead.exe
bin\x64\Debug\chair\chair.exe
bin\x64\Debug\cube_1\cube_1.exe
bin\x64\Debug\texCube\texCube.exe
bin\x64\Debug\stencilmirrorshadow\stencilmirrorshadow.exe
```

按 `ESC` 退出窗口。  
纹理示例可以用方向键切换贴图并调整相机。  
`stencilmirrorshadow` 包含正常场景、模板镜面和透明阴影三个绘制阶段。

## 当前可运行示例

当前保留 20 个可运行示例：

| 示例 | 状态 | 说明 |
|---|---|---|
| `triangle` | `accurate` | 彩色三角形 |
| `arrowhead` | `accurate` | 箭头几何体 |
| `chair` | `accurate` | 主椅子组合几何体 |
| `chair_1` | `variant` | 紧凑木椅变体 |
| `chair_2` | `variant` | 带坐垫椅子变体 |
| `chair_3` | `variant` | 梯背椅子变体 |
| `cube` | `variant` | 主旋转立方体 |
| `cube_1` | `accurate` | 旧版彩色旋转立方体 |
| `cube_2` | `variant` | 偏移长方体变体 |
| `cube_3` | `variant` | X/Y 旋转变体 |
| `cube_4` | `variant` | 非均匀缩放立方体 |
| `cube_5` | `variant` | 双立方体变换示例 |
| `d3dxcreate` | `accurate` | 运行时创建网格示例 |
| `octahedron` | `accurate` | 八面体 |
| `tetrahedron` | `accurate` | 四面体 |
| `tetrahedron_1` | `variant` | 拉伸四面体变体 |
| `texCube` | `variant` | 两个不同 UV 密度的纹理立方体 |
| `texCube_1` | `accurate` | 单个纹理立方体，支持方向键切换贴图 |
| `texCube_2` | `variant` | 重复 UV 的纹理长方体 |
| `stencilmirrorshadow` | `accurate` | 模板镜面和透明阴影 |

迁移状态记录在 `samples/samples.json`：

- `accurate`：D3D11 示例保留了旧文件的主要教学点。
- `variant`：同一类示例的小实验版本。
- `placeholder`：当前不允许出现在可运行示例里。
- `utility`：工具代码，不生成可运行示例。

## 不再生成 exe 的旧文件

`d3dUtility*` 是旧 D3D9 工具/框架代码，不是独立教学场景，所以不再生成 exe。  
这些文件保留在本地 `legacy/` 目录中作为参考，并通过 `.gitignore` 忽略。

高相似度、像备份的小改版本也不再生成 exe：

- `arrowhead_1`
- `arrowhead_2`
- `d3dxcreate_3`
- `octahedron_1`

它们的旧源码同样只保留在本地 `legacy/` 中。

## 自动启动测试

设置 `D3D_SAMPLE_SMOKE=1` 后，示例会自动运行几帧并退出：

```powershell
$env:D3D_SAMPLE_SMOKE='1'
Get-Content samples\samples.json | ConvertFrom-Json | ForEach-Object {
  $exe = "bin\x64\Debug\$($_.Name)\$($_.Name).exe"
  Start-Process -FilePath $exe -Wait
}
```

## 目录结构

- `common/`：共享 D3D11 窗口、设备、shader、buffer、texture 和场景代码
- `samples/`：每个可运行示例一个 Visual Studio 项目
- `assets/`：运行时编译的 HLSL 和生成的 32-bit BMP 纹理
- `legacy/`：旧 Direct3D 9 源码，仅本地参考，不提交
