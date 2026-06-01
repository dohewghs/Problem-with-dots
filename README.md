# 3D Reconstruction Clustering Engine

A high-performance spatial analysis tool designed to identify, filter, 
and calculate high-density point cloud clusters (centroids) from noisy data. 
This project serves as a practical sandbox for mastering low-level optimization, memory safety, and multi-threaded concurrency in native C++.

## 🎯 Project Goals & Main Idea
In 3D reconstruction pipelines (such as LiDAR scanning or photogrammetry), raw point clouds are
often flooded with environmental noise and floating artifacts. To reconstruct geometry accurately,
the system must fast-track the extraction of dense geometric clusters.

The main idea of this project is to implement an efficient **Two-Stage Spatial Grid Clustering Algorithm** that 
isolates heavy point concentrations (strong averages) from background noise (weak averages), initially built as a single-threaded 
baseline and systematically migrated toward a highly concurrent architecture.

---

## 🛠️ How It Works (Algorithm Breakdown)

The computational core leverages a custom **Spatial Grid (Axis-Aligned Bounding Boxes)** to achieve an optimized $O(N)$ runtime mapping phase, replacing the naive $O(N^2)$ brute-force approach.

1. **Grid Mapping (Spatial Grid):** The bounding surface is partitioned into a uniform $M \times M$ grid. Points are mathematically bucketed into cell vectors using a stable integer coordinate pair `std::pair<int, int>` as a map key to completely eliminate floating-point scaling drift.
2. **Stage 1: Weak Centroids:** For each active cell, a raw arithmetic mean ($\text{weak\_avg}$) is computed based on all contained points.
3. **Stage 2: Strong Centroids (Circle/Sphere Filtering):** A secondary radial pass (using pre-calculated squared radius bounds to bypass expensive `std::sqrt` calls) filters out single-point noise. If a cell contains a dense cluster exceeding a user-defined threshold, it refines the coordinates into a high-density cluster center ($\text{strong\_avg}$).

---

## 🏗️ Architecture & Clean Design
The codebase enforces strict **Separation of Concerns (RoC)** and is engineered from day one with **Dependency Injection** to keep execution modules isolated from state containers:

* **`Zone` / `Point`:** Native geometric primitives utilizing explicit `double` precision to guarantee sub-pixel coordinate accuracy.
* **`Points`:** A custom data container exposing overloaded stream/compound operators (`operator+=`) for clean data ingestion.
* **`Checker`:** The pure, state-independent math engine. It contains zero global states or singletons, making it fundamentally **thread-safe** for multi-threaded invocations.
* **`GuiManager` (Immediate Mode UI):** Integrated with **SDL3** and **Dear ImGui** to provide real-time runtime control over clustering parameters.

---

## 🎛️ Live Parameter Tuning (Immediate Mode UI)
Thanks to the decoupled design, you can dynamically tweak the spatial configuration on the fly and watch the cluster centers actively "snap" into place:
* **Grid Density (Zones X/Y):** Dynamically scales the underlying spatial buckets.
* **Search Radius:** Adjusts the mathematical circle boundary for localized density inspection.
* **Minimum Point Threshold:** Filters out arbitrary noise points from triggering false cluster heads.
* **Camera Zoom:** Implements viewport scale matrices via `SDL_SetRenderScale` to closely inspect dense micro-clusters.

---

## 🚀 Concurrency Roadmap (Next Steps)
While the current version achieves optimal performance on a single thread, the architecture was intentionally designed to adapt to **Parallel Processing**:

* [x] **Single-Threaded Baseline:** Establish clean mathematics, UI integration, and floating-point stability.
* [ ] **Data-Parallel Mapping:** Parallelize the initial point-cloud classification using data-splitting structures.
* [ ] **Thread & Mutex Isolation:** Implement safe concurrent writes to the sparse spatial map cells using localized mutex guards or lock-free strategies.
* [ ] **Task-Based Grid Analysis:** Distribute the Stage 2 radial centroid calculations across thread pools utilizing modern `std::jthread` (C++20).

---

## 💻 Tech Stack
* **Language:** C++20 / C++23
* **Graphics & Windowing:** SDL3 (Native Software/Hardware Accelerated Renderer)
* **User Interface:** Dear ImGui (Immediate Mode)
* **Build System:** CMake
