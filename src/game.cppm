export module game;
import ginseng;
import raylib;
import std;
import config;
import debug;

export {
    class game;
    class scene;

    class scene
    {
      public:
        scene(game& game);
        ginseng::database *m_world{nullptr};
        game *m_game{nullptr};
        std::span<debug::message> m_debug_messages;
        bool m_should_exit = false;

        [[nodiscard]] bool should_exit() const
        {
            return m_should_exit;
        }
        [[nodiscard]] virtual bool should_exit_game() const
        {
            return false;
        }

        virtual void on_start() = 0;
        virtual void on_update() = 0;
        virtual void on_render() = 0;
        virtual std::unique_ptr<scene> on_exit() = 0;
        virtual ~scene() = default;
    };

    class game
    {
      public:
        game(config::app_info spec);
        void exit();
        auto get_world() -> ginseng::database &
        {
            return m_world;
        }
        void set_scene(std::unique_ptr<scene> scene);
        [[nodiscard]] int run();
        auto get_debug_messages() -> std::vector<debug::message> &
        {
            return m_debug_messages;
        }
        [[nodiscard]] auto get_spec() const -> const config::app_info
        {
            return m_spec;
        }

      private:
        config::app_info m_spec;
        std::unique_ptr<scene> m_scene;
        ginseng::database m_world;
        std::vector<debug::message> m_debug_messages;
    };
}
