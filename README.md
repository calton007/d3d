# D3D11 Teaching Samples

This repository modernizes old Direct3D 9 sample files into runnable Direct3D 11 Win32 teaching samples.

## Requirements

- Windows
- Visual Studio Build Tools with C++ tools
- Windows 10/11 SDK

The project uses Direct3D 11, DirectXMath, and runtime-compiled HLSL. It does not require the June 2010 DirectX SDK or any third-party library.

## Build

Open a Visual Studio Developer Command Prompt, then run:

```bat
msbuild d3d.sln /p:Configuration=Debug /p:Platform=x64
```

If `msbuild` is not on `PATH`, run it through `VsDevCmd.bat`:

```bat
cmd /c "call ""C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"" -arch=x64 -host_arch=x64 && msbuild d3d.sln /p:Configuration=Debug /p:Platform=x64"
```

## Run

Each sample builds as a separate executable:

```bat
bin\x64\Debug\<sample>\<sample>.exe
```

Examples:

```bat
bin\x64\Debug\triangle\triangle.exe
bin\x64\Debug\arrowhead\arrowhead.exe
bin\x64\Debug\chair\chair.exe
bin\x64\Debug\cube_1\cube_1.exe
bin\x64\Debug\texCube\texCube.exe
bin\x64\Debug\stencilmirrorshadow\stencilmirrorshadow.exe
```

Press `ESC` to exit. Texture samples use arrow keys to switch textures and adjust camera height/orbit. `stencilmirrorshadow` renders a normal scene pass, a stencil mirror pass, and an alpha shadow pass.

## Current runnable samples

There are 20 runnable samples:

| Sample | Status | Notes |
|---|---|---|
| `triangle` | `accurate` | Colored triangle |
| `arrowhead` | `accurate` | Main arrowhead mesh |
| `chair` | `accurate` | Main composite chair geometry |
| `chair_1` | `variant` | Compact wood chair geometry |
| `chair_2` | `variant` | Cushioned chair geometry |
| `chair_3` | `variant` | Ladder-back chair geometry |
| `cube` | `variant` | Main rotating cube |
| `cube_1` | `accurate` | Legacy colored rotating cube |
| `cube_2` | `variant` | Offset rectangular box volume |
| `cube_3` | `variant` | X/Y rotation transform variant |
| `cube_4` | `variant` | Non-uniform scaled cube variant |
| `cube_5` | `variant` | Two-cube transform variant |
| `d3dxcreate` | `accurate` | Runtime-created mesh demonstration |
| `octahedron` | `accurate` | Main octahedron mesh |
| `tetrahedron` | `accurate` | Main tetrahedron mesh |
| `tetrahedron_1` | `variant` | Stretched tetrahedron variant |
| `texCube` | `variant` | Two textured cubes with different UV density |
| `texCube_1` | `accurate` | Single textured cube with arrow-key texture switching |
| `texCube_2` | `variant` | Repeated UV textured box |
| `stencilmirrorshadow` | `accurate` | Stencil mirror plus alpha shadow |

Migration status is tracked in `samples/samples.json`:

- `accurate`: the D3D11 sample carries the old file's main teaching point.
- `variant`: the file is a small experiment or variant of a main sample.
- `placeholder`: not allowed for active runnable samples.
- `utility`: not generated as a runnable sample.

## Removed from runnable samples

`d3dUtility*` legacy files are not generated as executables. They are retained under `legacy/` as D3D9 framework/reference code because they are utilities, not standalone teaching scenes.

High-similarity backup-like samples are also not generated as executables: `arrowhead_1`, `arrowhead_2`, `d3dxcreate_3`, and `octahedron_1`. Their legacy sources remain under `legacy/` for reference.

## Smoke test

Set `D3D_SAMPLE_SMOKE=1` to make each sample close automatically after a few frames:

```powershell
$env:D3D_SAMPLE_SMOKE='1'
Get-Content samples\samples.json | ConvertFrom-Json | ForEach-Object {
  $exe = "bin\x64\Debug\$($_.Name)\$($_.Name).exe"
  Start-Process -FilePath $exe -Wait
}
```

## Layout

- `common/`: shared D3D11 window, device, shader, buffer, texture, and scene code
- `samples/`: one Visual Studio project per executable
- `assets/`: runtime-compiled HLSL and generated 32-bit BMP textures
- `legacy/`: original Direct3D 9 source files kept for reference
