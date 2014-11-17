////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class GLSurface : public Awesomium::Surface {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  GLSurface(unsigned width, unsigned height)
    : tex_(nullptr)
    , buffer_(width * height * 4)
    , width_(width)
    , height_(height)
    , needs_update_(false) {}

  ~GLSurface() {}

  // ------------------------------------------------------------ public methods

  //////////////////////////////////////////////////////////////////////////////

  void init(RenderContext const& ctx) {
    std::unique_lock<std::mutex> lock(mutex_);
    tex_ = std::make_shared<Texture2D>(
      width_, height_,
      scm::gl::FORMAT_RGBA_8
    );
    tex_->upload_to(ctx);
  }

  //////////////////////////////////////////////////////////////////////////////

  bool bind(RenderContext const& ctx) {

    if (!tex_) {
      init(ctx);
    }

    ctx.render_context->current_program()->uniform("gua_in_texture", tex_->get_handle(ctx));

    if (needs_update_) {
      std::unique_lock<std::mutex> lock(mutex_);
      needs_update_ = false;
      tex_->update_sub_data(
        ctx,
        scm::gl::texture_region(math::vec3ui(0, 0, 0), math::vec3ui(width_, height_, 1)),
        0u, scm::gl::FORMAT_BGRA_8, &buffer_.front()
      );
    }

    return true;
  }

  //////////////////////////////////////////////////////////////////////////////

  void Paint(unsigned char* src_buffer, int src_row_span,
             Awesomium::Rect const& src_rect,
             Awesomium::Rect const& dest_rect) {

    std::unique_lock<std::mutex> lock(mutex_);

    for (int row = 0; row < dest_rect.height; row++) {
      memcpy(&buffer_.front() + (row + dest_rect.y) * width_*4 + (dest_rect.x * 4),
             src_buffer + (row + src_rect.y) * src_row_span + (src_rect.x * 4),
             dest_rect.width * 4);
    }

    needs_update_ = true;
  }

  //////////////////////////////////////////////////////////////////////////////

  void Scroll(int dx, int dy, Awesomium::Rect const& clip_rect) {

    if (abs(dx) >= clip_rect.width || abs(dy) >= clip_rect.height) {
      return;
    }

    std::unique_lock<std::mutex> lock(mutex_);

    if (dx < 0 && dy == 0) {
      // Area shifted left by dx
      unsigned char* tempBuffer = new unsigned char[(clip_rect.width + dx) * 4];

      for (int i = 0; i < clip_rect.height; i++) {
        memcpy(tempBuffer, &buffer_.front() + (i + clip_rect.y) * width_*4 +
               (clip_rect.x - dx) * 4, (clip_rect.width + dx) * 4);
        memcpy(&buffer_.front() + (i + clip_rect.y) * width_*4 + (clip_rect.x) * 4,
               tempBuffer, (clip_rect.width + dx) * 4);
      }

      delete[] tempBuffer;

    } else if (dx > 0 && dy == 0) {
      // Area shifted right by dx
      unsigned char* tempBuffer = new unsigned char[(clip_rect.width - dx) * 4];

      for (int i = 0; i < clip_rect.height; i++) {
        memcpy(tempBuffer, &buffer_.front() + (i + clip_rect.y) * width_*4 +
               (clip_rect.x) * 4, (clip_rect.width - dx) * 4);
        memcpy(&buffer_.front() + (i + clip_rect.y) * width_*4 + (clip_rect.x + dx) * 4,
               tempBuffer, (clip_rect.width - dx) * 4);
      }

      delete[] tempBuffer;

    } else if (dy < 0 && dx == 0) {
      // Area shifted down by dy
      for (int i = 0; i < clip_rect.height + dy ; i++)
        memcpy(&buffer_.front() + (i + clip_rect.y) * width_*4 + (clip_rect.x * 4),
               &buffer_.front() + (i + clip_rect.y - dy) * width_*4 + (clip_rect.x * 4),
               clip_rect.width * 4);
    } else if (dy > 0 && dx == 0) {
      // Area shifted up by dy
      for (int i = clip_rect.height - 1; i >= dy; i--)
        memcpy(&buffer_.front() + (i + clip_rect.y) * width_*4 + (clip_rect.x * 4),
               &buffer_.front() + (i + clip_rect.y - dy) * width_*4 + (clip_rect.x * 4),
               clip_rect.width * 4);
    }

    needs_update_ = true;
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::shared_ptr<gua::Texture2D> tex_;

  std::vector<unsigned char>  buffer_;

  unsigned   width_;
  unsigned   height_;
  std::mutex mutex_;
  bool       needs_update_;
};
