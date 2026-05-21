CREATE DATABASE board;

-- 사용자 테이블: 아이디(중복 불가), 비밀번호 해시, 가입 시각
CREATE TABLE users (
  id         INT PRIMARY KEY AUTO_INCREMENT,
  username   VARCHAR(50)  NOT NULL UNIQUE,
  password   VARCHAR(255) NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 게시글 테이블: 제목, 본문, 작성자(users 참조), 작성·수정 시각
CREATE TABLE posts (
  id         INT PRIMARY KEY AUTO_INCREMENT,
  title      VARCHAR(200) NOT NULL,
  content    TEXT,
  author_id  INT NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
                      ON UPDATE CURRENT_TIMESTAMP,
  FOREIGN KEY (author_id) REFERENCES users(id)
);

-- 댓글 테이블: 어느 글(post_id)에, 누가(author_id), 무슨 내용을 남겼는지
CREATE TABLE comments (
  id         INT PRIMARY KEY AUTO_INCREMENT,
  post_id    INT NOT NULL,
  author_id  INT NOT NULL,
  content    TEXT NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (post_id)   REFERENCES posts(id),
  FOREIGN KEY (author_id) REFERENCES users(id)
);

-- 첨부파일 테이블: 어느 글에 붙은 파일인지 + 원본 이름·저장 경로·크기
CREATE TABLE attachments (
  id            INT PRIMARY KEY AUTO_INCREMENT,
  post_id       INT NOT NULL,
  original_name VARCHAR(255) NOT NULL,
  stored_path   VARCHAR(500) NOT NULL,
  size_bytes    BIGINT,
  created_at    DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (post_id) REFERENCES posts(id)
);
