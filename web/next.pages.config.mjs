/** @type {import('next').NextConfig} */
const nextConfig = {
  output: "export",
  basePath: "/project-wrover", // build for gh-pages
  assetPrefix: "/project-wrover",
  distDir: "build",
};

export default nextConfig;
