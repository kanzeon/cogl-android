#include <cogl/cogl.h>
#include <glib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static void
update_cogl_x11_event_mask (CoglOnscreen *onscreen,
                            guint32 event_mask,
                            void *user_data)
{
  Display *xdpy = user_data;
  XSetWindowAttributes attrs;
  guint32 xwin;

  attrs.event_mask = event_mask;
  xwin = cogl_onscreen_x11_get_window_xid (onscreen);

  XChangeWindowAttributes (xdpy,
                           (Window)xwin,
                           CWEventMask,
                           &attrs);
}

int
main (int argc, char **argv)
{
  Display *xdpy;
  CoglRenderer *renderer;
  CoglSwapChain *chain;
  CoglOnscreenTemplate *onscreen_template;
  CoglDisplay *display;
  CoglContext *ctx;
  CoglOnscreen *onscreen;
  CoglFramebuffer *fb;
  GError *error = NULL;
  guint32 visual;
  XVisualInfo template, *xvisinfo;
  int visinfos_count;
  XSetWindowAttributes xattr;
  unsigned long mask;
  Window xwin;

  /* Since we want to test external ownership of the X display,
   * connect to X manually... */
  xdpy = XOpenDisplay (NULL);
  if (!xdpy)
    {
      fprintf (stderr, "Failed to open X Display\n");
      return 1;
    }

  /* Conceptually choose a GPU... */
  renderer = cogl_renderer_new ();
  /* FIXME: This should conceptually be part of the configuration of
   * a renderer. */
  cogl_renderer_xlib_set_foreign_display (renderer, xdpy);
  if (!cogl_renderer_connect (renderer, &error))
    {
      fprintf (stderr, "Failed to connect to a renderer: %s\n",
               error->message);
    }

  chain = cogl_swap_chain_new ();
  cogl_swap_chain_set_has_alpha (chain, TRUE);

  /* Conceptually declare upfront the kinds of windows we anticipate
   * creating so that when we configure the display pipeline we can avoid
   * having an impedance miss-match between the format of windows and the
   * format the display pipeline expects. */
  onscreen_template = cogl_onscreen_template_new (chain);
  cogl_object_unref (chain);

  /* Conceptually setup a display pipeline */
  display = cogl_display_new (renderer, onscreen_template);
  cogl_object_unref (renderer);
  if (!cogl_display_setup (display, &error))
    {
      fprintf (stderr, "Failed to setup a display pipeline: %s\n",
               error->message);
      return 1;
    }

  ctx = cogl_context_new (display, &error);
  if (!ctx)
    {
      fprintf (stderr, "Failed to create context: %s\n", error->message);
      return 1;
    }
  /* Eventually we want to get rid of any "default context" but for now it's
   * needed...  */
  cogl_set_default_context (ctx);

  onscreen = cogl_onscreen_new (ctx, 640, 480);

  /* We want to test that Cogl can handle foreign X windows... */

  visual = cogl_onscreen_x11_get_visual_xid (onscreen);
  if (!visual)
    {
      fprintf (stderr, "Failed to query an X visual suitable for the "
               "configured CoglOnscreen framebuffer\n");
      return 1;
    }

  template.visualid = visual;
  xvisinfo = XGetVisualInfo (xdpy, VisualIDMask, &template, &visinfos_count);

  /* window attributes */
  xattr.background_pixel = WhitePixel (xdpy, DefaultScreen (xdpy));
  xattr.border_pixel = 0;
  xattr.colormap = XCreateColormap (xdpy,
                                    DefaultRootWindow (xdpy),
                                    xvisinfo->visual,
                                    AllocNone);
  mask = CWBorderPixel | CWColormap;

  xwin = XCreateWindow (xdpy,
                        DefaultRootWindow (xdpy),
                        0, 0,
                        800, 600,
                        0,
                        xvisinfo->depth,
                        InputOutput,
                        xvisinfo->visual,
                        mask, &xattr);

  XFree (xvisinfo);

  cogl_onscreen_x11_set_foreign_window_xid (onscreen, xwin,
                                            update_cogl_x11_event_mask,
                                            xdpy);

  fb = COGL_FRAMEBUFFER (onscreen);
  /* Eventually there will be an implicit allocate on first use so this
   * will become optional... */
  if (!cogl_framebuffer_allocate (fb, &error))
    {
      fprintf (stderr, "Failed to allocate framebuffer: %s\n", error->message);
      return 1;
    }

  XMapWindow (xdpy, xwin);

  cogl_push_framebuffer (fb);

  cogl_set_source_color4f (1, 0, 0, 1);
  for (;;)
    {
      cogl_rectangle (-1, 1, 1, -1);
      cogl_framebuffer_swap_buffers (fb);
    }

  return 0;
}
