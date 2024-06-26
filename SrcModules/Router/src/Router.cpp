#include "Router.hh"

nzm::Router::Router() {}

nzm::Router::~Router() {}

void nzm::Router::execRouting(zia::api::HttpDuplex &httpDuplex) {
  // Pre routing / rewrite
  // Not now ...
  // Route to URI physical
  if (!uriFound("." + httpDuplex.req.uri)) {
    display404(httpDuplex);
  } else if (isDirectory("." + httpDuplex.req.uri)) {
    // Check index file
    // File directory
    displayBrowsing(httpDuplex);
  } else {
    // Get data into file and put inside body
    displayFile(httpDuplex);
  }
}

bool nzm::Router::uriFound(std::string uri) const {
  return std::filesystem::exists(uri);
}

bool nzm::Router::isDirectory(std::string uri) const {
  return (std::filesystem::is_directory(uri));
}

std::string nzm::Router::getTypeFile(std::string uri) const {
  return std::string("text/html");
}

void nzm::Router::display404(zia::api::HttpDuplex &httpDuplex) {
  // Settings base
  httpDuplex.resp.version = zia::api::http::Version::http_1_1;
  httpDuplex.resp.status = zia::api::http::common_status::not_found;
  httpDuplex.resp.reason = "Not Found";

  // Body
  std::string content = "<html>"
                        "<head>"
                        "<title>Index of " +
                        httpDuplex.req.uri +
                        "</title>"
                        "</head>"
                        "<body>"
                        "<h1>Not found " +
                        httpDuplex.req.uri + "</h1>";

  content += "File / Directory not found into server";

  content += "</body></html>";

  for (char i : content) {
    httpDuplex.resp.body.push_back(std::byte(i));
  }

  // Headers values
  httpDuplex.resp.headers["Content-Type"] = "text/html";
  httpDuplex.resp.headers["Content-Length"] =
      std::to_string(httpDuplex.resp.body.size());
}

// Add in 2024 - Part of the C++17 to C++20 upgrade
std::time_t to_time_t(std::filesystem::file_time_type ftime) {
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(
      ftime - std::filesystem::file_time_type::clock::now() +
      system_clock::now());
  return system_clock::to_time_t(sctp);
}

void nzm::Router::displayBrowsing(zia::api::HttpDuplex &httpDuplex) {
  // Settings base
  httpDuplex.resp.version = zia::api::http::Version::http_1_1;
  httpDuplex.resp.status = zia::api::http::common_status::ok;
  httpDuplex.resp.reason = "OK";

  // Body
  std::string content = "<html>"
                        "<head>"
                        "<title>Index of " +
                        httpDuplex.req.uri +
                        "</title>"
                        "</head>"
                        "<body>"
                        "<h1>Index of " +
                        httpDuplex.req.uri + "</h1>";

  content += "<table cellspacing=\"10\">"
             "<tbody><tr><th>Name</th><th>Last modified</th><th>Size</th></tr>";

  if (httpDuplex.req.uri != "/") {
    content += "<tr>";
    // Name
    content += "<td><a href=\"..\">..</a></td>";
    // Last modified
    content += "<td>-</td>";
    // Size
    content += "<td>-</td>";
    content += "</tr>";
  }
  for (auto &p :
       std::filesystem::directory_iterator("." + httpDuplex.req.uri)) {
    content += "<tr>";
    // Name
    content += "<td><a href=\"" + httpDuplex.req.uri;
    if (httpDuplex.req.uri.length() > 1) {
      content += "/";
    }
    content += std::filesystem::path(p).filename().string() + "\">" +
               std::filesystem::path(p).filename().string() + "</a></td>";
    // Last modified
    auto ftime = std::filesystem::last_write_time(p);
    // Next line, remove in 2024 during the upgrade from C++17 to C++20
    // std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); //
    // assuming system_clock
    std::time_t cftime = to_time_t(ftime);

    content +=
        "<td>" + std::string(std::asctime(std::localtime(&cftime))) + "</td>";
    // Size
    if (std::filesystem::is_regular_file(p)) {
      content +=
          "<td>" + std::to_string(std::filesystem::file_size(p)) + "</td>";
    } else {
      content += "<td>-</td>";
    }
    content += "</tr>";
  }

  content += "</tbody></table>";

  content += "</body></html>";

  for (char i : content) {
    httpDuplex.resp.body.push_back(std::byte(i));
  }

  // Headers values
  httpDuplex.resp.headers["Content-Type"] = "text/html";
  httpDuplex.resp.headers["Content-Length"] =
      std::to_string(httpDuplex.resp.body.size());
}

void nzm::Router::displayFile(zia::api::HttpDuplex &httpDuplex) {
  // Settings base
  httpDuplex.resp.version = zia::api::http::Version::http_1_1;
  httpDuplex.resp.status = zia::api::http::common_status::ok;
  httpDuplex.resp.reason = "OK";

  // Data file
  std::ifstream fileUri(("." + httpDuplex.req.uri).c_str(),
                        std::ifstream::binary);
  std::string content((std::istreambuf_iterator<char>(fileUri)),
                      std::istreambuf_iterator<char>());

  for (char i : content) {
    httpDuplex.resp.body.push_back(std::byte(i));
  }

  // Headers values
  httpDuplex.resp.headers["Content-Length"] =
      std::to_string(httpDuplex.resp.body.size());
}
