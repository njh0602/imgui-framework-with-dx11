#pragma once

// c++
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <bitset>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <iterator>
#include <new>
#include <random>
#include <thread>
#include <chrono>
#include <future>


// base
#include "base/Application.h"
#include "base/AutoreleasePool.h"
#include "base/Director.h"
#include "base/MeshLoader.h"
#include "base/MeshCache.h"
#include "base/Ref.h"
#include "base/Vector.h"
#include "base/Data.h"


// math
#include "math/Mat4.h"
#include "math/MathBase.h"
#include "math/MathUtil.h"
#include "math/Quaternion.h"
#include "math/Vec3.h"
#include "math/Vec4.h"


// utils
#include "utils/Random.h"


// renderer
#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"
#include "renderer/Camera.h"
#include "renderer/DefaultShader.h"
#include "renderer/ShaderProgram.h"
#include "renderer/ShaderProgramCache.h"
#include "renderer/TriangleCommand.h"
#include "renderer/CustomCommand.h"
#include "renderer/Types.h"
#include "renderer/Texture2D.h"
#include "renderer/TextureCache.h"
#include "renderer/Image.h"


// node
#include "node/Mesh.h"
#include "node/Model3D.h"
#include "node/Node.h"
#include "node/Scene.h"
#include "node/ImGuiLayer.h"

